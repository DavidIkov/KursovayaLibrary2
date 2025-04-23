#include"Obj.h"
#include"Maths/Vector.h"
#include<limits>
#include"KE2_Manager.h"
#include<fstream>
#include"Tools/Time.h"

static unsigned int findSymbolInd(const std::string& txt, const char symbol, const unsigned int startInd) {
	unsigned int i = startInd;
	while (txt[i] != symbol) {
		if (txt[i] == '\0') return (unsigned int)txt.size();
		i++;
	}
	return i;
}
static int mini(int a1, int a2) {
	return (a1 > a2) ? a2 : a1;
}

//TODO: add exceptions and some safety
//TODO: fix problem with repeating vertexes(good example in helicopter.obj)

/*how algorithm works
1. calculate amount of vertexes,normals,connections
2. save vertexes,normals,connections
3. calculate face normals and smoothed normals
*/

//return packed data: Position,SmoothedNormal,FaceNormal,TextureCords
std::vector<float> ReadObjFileType(const char* filePath) {

	std::fstream file(filePath, std::ios::in);
	std::string curLine;

	if (KE2::Manager::OutputStream != nullptr) *KE2::Manager::OutputStream << "Starting processing .obj file \"" << filePath << "\"...  ";

	float startTime = TimeNamespace::GetTime();

	unsigned int fileVertexesAmount = 0;
	unsigned int fileVertexConnectionsAmount = 0;
	unsigned int fileVertexConnectionsLensAmount = 0;
	std::vector<Vector3F> fileVertexes;
	std::vector<unsigned int> fileVertexConnections;//how much is there "a/b/c"
	std::vector<unsigned int> fileVertexConnectionsLens;

	//first step
	{
		while (std::getline(file,curLine)) {

			std::string name = curLine.substr(0, findSymbolInd(curLine, ' ', 0));
			if (name == "v")//vertex
				fileVertexesAmount++;
			else if (name == "f") {//vertexes order
				fileVertexConnectionsLensAmount++;
				for (unsigned int i = 1; i < curLine.length() - 1; i++)
					if (curLine[i] == ' ') fileVertexConnectionsAmount++;
			}
		
		}

		fileVertexes.reserve(fileVertexesAmount);
		fileVertexConnections.reserve(fileVertexConnectionsAmount * 3);
		fileVertexConnectionsLens.reserve(fileVertexConnectionsLensAmount);

	}
	
	file.clear(); file.seekg(0);

	unsigned int trianglesAmount = 0;

	//second step
	{
		while (std::getline(file,curLine)) {

			std::string name = curLine.substr(0, findSymbolInd(curLine, ' ', 0));
			if (name == "v") {//vertex
				//example of data: "v -0.276388 -0.447220 0.850649"
				unsigned int si = 2;
				float nums[3] = { 0,0,0 };
				unsigned int cordInd = 0;
				for (unsigned int ci = 3;ci < curLine.length()+1; ci++) {
					if (curLine[ci] == ' ' or ci == curLine.length()) {
						nums[cordInd] = std::stof(curLine.substr(si, ci - 1));
						cordInd++;
						si = ci + 1;
					}
				}
				fileVertexes.push_back(Vector3F(nums[0],nums[1],nums[2]));
			}
			else if (name == "f") {

				/*example of data:
				f 98/2/80 86/4/80 99/18/80
				f 131/22/91 132/60/91 146/60/91 137/64/91
				note that indexes in .obj starts from 1 instead of 0
				*/

				unsigned int si = 1;
				unsigned int len = 0;
				bool waitingForNum = true;
				for (unsigned int ci = 2; ci < curLine.length(); ci++) {
					if (curLine[ci] == ' ') {
						si = ci;
						waitingForNum = true;
					}
					else if (curLine[ci] == '/' and waitingForNum) {
						fileVertexConnections.push_back(std::stoi(curLine.substr(si + 1, ci - si - 1)) - 1);
						waitingForNum = false;
						len++;
					}
				}
				fileVertexConnectionsLens.push_back(len);

				trianglesAmount += len - 2;

			}

		}
	}


	std::vector<float> preparedData;
	preparedData.reserve((3 + 3 + 3 + 2) * trianglesAmount);

	//third step
	{
		
		std::vector<float> smoothedNormalsMins(fileVertexesAmount * 3, nanf(""));
		std::vector<float> smoothedNormalsMaxs(fileVertexesAmount * 3, nanf(""));

		unsigned int orderForThree[] = { 0,1,2 };
		unsigned int orderForFour[] = { 0,1,2,2,3,0 };

		unsigned int curConOff = 0;
		for (unsigned int coi = 0; coi < fileVertexConnectionsLens.size(); coi++) {
			unsigned int len = fileVertexConnectionsLens[coi];
			unsigned int curTrianglesAmount = len - 2;
			unsigned int* order = orderForThree;
			if (len == 4) order = orderForFour;
			//else if (len != 3) DebuggingTools::ManageTheError({ DebuggingTools::ErrorTypes::Critical,".OBJ FILE GOT MORE THEN FOUR OR LESS THEN 3 CONNECTIONS",KURSAVAYAENGINE2_CORE_ERRORS::TRYING_TO_CALL_FUNCTION_WITH_INVALID_ARGUMENTS });
			
			Vector3F normal = (fileVertexes[fileVertexConnections[curConOff + order[1]]] - fileVertexes[fileVertexConnections[curConOff + order[0]]]).Cross
			(fileVertexes[fileVertexConnections[curConOff + order[2]]] - fileVertexes[fileVertexConnections[curConOff + order[0]]]);
			Vector3F unitNormal = normal.Normalize();

			for (unsigned int to = 0; to < curTrianglesAmount; to++) {
				unsigned int vi[] = {
					fileVertexConnections[curConOff + order[to * 3 + 0]],
					fileVertexConnections[curConOff + order[to * 3 + 1]],
					fileVertexConnections[curConOff + order[to * 3 + 2]],
				};

				for (unsigned int vii = 0; vii < 3; vii++) {
					//pos
					preparedData.push_back(fileVertexes[vi[vii]][0]);
					preparedData.push_back(fileVertexes[vi[vii]][1]);
					preparedData.push_back(fileVertexes[vi[vii]][2]);

					//keep smoothed normal empty for now, first element will be index of smoothed vec
					preparedData.push_back((float)vi[vii]);
					preparedData.push_back(0);
					preparedData.push_back(0);

					//face normal
					preparedData.push_back(unitNormal[0]);
					preparedData.push_back(unitNormal[1]);
					preparedData.push_back(unitNormal[2]);

					for (unsigned int nii = 0; nii < 3; nii++) {
						unsigned int wi = vi[vii] * 3 + nii;
						if (isnan(smoothedNormalsMaxs[wi])) {
							smoothedNormalsMaxs[wi] = unitNormal[nii];
							smoothedNormalsMins[wi] = unitNormal[nii];
						}
						else {
							if (unitNormal[nii] > smoothedNormalsMaxs[wi]) smoothedNormalsMaxs[wi] = unitNormal[nii];
							else if (unitNormal[nii] < smoothedNormalsMins[wi]) smoothedNormalsMins[wi] = unitNormal[nii];
						}
					}

					//texture cords
					preparedData.push_back(0);
					preparedData.push_back(0);
				}
			}


			curConOff += len;
		}

		for (unsigned int pdi = 3; pdi < preparedData.size(); pdi += 11) {
			unsigned int smoothedNormalSInd = (unsigned int)preparedData[pdi] * 3;
			Vector3F unitVec(
				(smoothedNormalsMins[smoothedNormalSInd + 0] + smoothedNormalsMaxs[smoothedNormalSInd + 0]) / 2,
				(smoothedNormalsMins[smoothedNormalSInd + 1] + smoothedNormalsMaxs[smoothedNormalSInd + 1]) / 2,
				(smoothedNormalsMins[smoothedNormalSInd + 2] + smoothedNormalsMaxs[smoothedNormalSInd + 2]) / 2
			);
			unitVec /= unitVec.Length();

			preparedData[pdi + 0] = unitVec[0];
			preparedData[pdi + 1] = unitVec[1];
			preparedData[pdi + 2] = unitVec[2];
		}
		/*for (unsigned int pdi = 3; pdi < preparedData.size(); pdi += 11) {
			unsigned int smoothedNormaInd = (unsigned int)preparedData[pdi];
			Vector3 unitVec = smoothedNormals[smoothedNormaInd].Unit();
			preparedData[pdi + 0] = unitVec[0];
			preparedData[pdi + 1] = unitVec[1];
			preparedData[pdi + 2] = unitVec[2];
		}*/
	}

	if (KE2::Manager::OutputStream != nullptr) *KE2::Manager::OutputStream << "Finished processing .obj in " << TimeNamespace::GetTime() - startTime << " seconds" << std::endl;



	return preparedData;
}
