#include "Simulation.h"

const std::vector<std::string*> split(const std::string &fileString, char c);


Simulation::Simulation(std::map<std::string,float> values){
	valuemap = std::map<std::string, float>(values);
	PNOT=getValue("PNOT");
	SPRING = getValue("SPRING");
	DT = getValue("DT");
	ACTIVE_TENSION = getValue("ACTIVE_TENSION");
	SUB_SAMPLE = (int)getValue("SUB_SAMPLE");
	STEPS_PER_FRAME =(int)getValue("STEPS_PER_FRAME");
	
}

Simulation* Simulation::loadSimulation(const std::string &simFileName){
	std::ifstream simFileStream(simFileName.c_str());
    std::stringstream simFileData;
    simFileData << simFileStream.rdbuf();
    simFileStream.close();
    
    std::vector<std::string*> lines = split(simFileData.str(),'\n');
	std::map<std::string, float> valuemap;
	
	//loop through lines and turn them into key/value pairs.
	int line_count = (int)lines.size();

	for(int i = 0; i<line_count; i++){
        
        std::string* line = lines[i];
        
        const std::vector<std::string*> ordinates = split(*line,'\t');
        
        if(ordinates.size()!=2){
            printf("bad config entry at line %d with %d members\n", i, (int)ordinates.size());
            for(int n = 0; n<ordinates.size(); n++){
				printf(ordinates[n]->c_str());
				printf("\n");
			}
            printf("trying to continue\n");
        }
        
        const std::string key(ordinates[0]->c_str());
        float value;
        sscanf(ordinates[1]->c_str(),"%f", &value);
		valuemap[key]=value;
    }
	
	
	return new Simulation(valuemap);
}

float Simulation::getValue(const char* key){
	return valuemap[std::string(key)];
}
/**
 * Breaks up a config file into lines and leaves out comments.
 * */
const std::vector<std::string*> split(const std::string &fileString, char c){
    std::vector<std::string*> pieces;
    
    const char* fileChars = fileString.c_str();
        
    int length = (int)fileString.length();
    int i,j;

    for(i = 0;i<length;i++){

        //starting a vertex.
        for(j = 0; j<length-i; j++){
                
            //found the splitter create a new string.
            if(fileChars[i+j]==c){

                if(j>0&&fileChars[i]!='#'){
                    char piece[j+1];
                    piece[j] = 0;           
                    for(int k = 0; k<j; k++){
                        piece[k] = fileChars[i+k];
                    }
                    std::string* sp = new std::string(piece);
                    pieces.push_back(sp);
                }
                //move to next word 
                i+=(j+1);
                
                //start over
                j = -1;
            }
        }
        
        //check if there is a last piece that needs to be added.
        if((j>0)&&(fileChars[i]!='#')){
            
            //create a null terminated string.
            char piece[j+1];
            piece[j] = 0;           
            for(int k = 0; k<j; k++){
                piece[k] = fileChars[i+k];
            }
            std::string* sp = new std::string(piece);
            pieces.push_back(sp);
            i+=(j+1);
        }
    }
    
    return pieces;
}
