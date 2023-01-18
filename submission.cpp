//include statements
#include <iostream>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

//a Player class to hold the two fields of a Player (id and score)
class Player{
	public:
	int id=0;
	int points=0;
};

//a function to swap the players in the heap
void swap(Player*a, Player*b){
	Player temp = *a;
	*a=*b;
	*b=temp;
}

//a function to swap the player heap positions in the handles array
void swapHandle(int *x, int *y){
	int temp = *x;
	*x = *y;
	*y=temp;
}

//a function to heapify the heap
void heapify(Player players[], int handles[], int numPlayers, int x){
	int smallest = x;
	
	//the index of the left and right child of the current player
	int left = 2*x+1;
	int right = 2*x+2;
	
	//determine if left child is smaller
	if(left<numPlayers&& players[left].points < players[smallest].points){
		smallest = left;	
	}
	
	//determine if right child is smaller
	if(right<numPlayers&& players[right].points < players[smallest].points){
		smallest = right;	
	}
	
	//swap values if one of the children was smaller, then recursive call
	if(smallest != x){
		swap(&players[x], &players[smallest]);
		swapHandle(&handles[players[x].id], &handles[players[smallest].id]);
		heapify(players,handles, numPlayers, smallest);	
	}
}

//the main function
int main(int argc, char *argv[]){
	ifstream input;
	ofstream output;
	
	//my temporary variables
	string line;
	string arg1;
	string arg2;
	int argi1;
	int argi2;
	int totalPlayers;
	int numPlayers=0;
	int in;
	int found;
	Player temp;
	
	//open the input and output files
	line = argv[1];
	input.open(line);
	line = argv[2];
	output.open(line);
	
	//store how many players can be put into the heap
	input >> totalPlayers;
	
	//make heap and handle array with totalPlayers
	Player players[totalPlayers];
	int handles[totalPlayers+1];
	
	//fill handle array with -1
	for (int i =0; i <totalPlayers+1; i++){handles[i]=-1;}
	
	//primer for the while loop
	input >> line;
	while(input){
		
		//insertContestant case
		if (line == "insertContestant"){
			
			//grab the two parameters from the input file and print line to output file
			input >>arg1;
			input >> arg2;
			output << line << " " << arg1 << " " << arg2 << endl;
			arg1 = arg1.substr(1, arg1.size()-2);
			arg2 = arg2.substr(1, arg2.size()-2);
			argi1=stoi(arg1);
			argi2=stoi(arg2);
			
			//check if player is already in the heap
			in =0;
			for(int i =0; i < numPlayers; i++){
				if (players[i].id == argi1){in =1;}			
			}
			
			//if already in the heap, print error message
			if (in ==1){
				output << "Contestant <"<<argi1<<"> is already in the extended heap: cannot insert." << endl;
			}
			
			//if the heap is full, print error message
			if(numPlayers==totalPlayers){
				output << "Contestant <"<<argi1<<"> could not be inserted because the extended heap is full." << endl;		
			}
			
			//otherwise, insert into the heap, update handle array, heapify if necessary, and print message
			else{
				temp.id = argi1;
				temp.points = argi2;
				players[numPlayers]=temp;
				handles[temp.id]=numPlayers;
				numPlayers++;
				in = numPlayers-1;
				while(in !=0 && players[(in-1)/2].points > players[in].points){
					swap(&players[in], &players[(in-1)/2]);
					swapHandle(&handles[players[in].id], &handles[players[(in-1)/2].id]);
					in = (in-1)/2;
				}
				output << "Contestant <"<<argi1<<"> inserted with initial score <"<<argi2<<">." << endl;
			}
			
		}
		
		//showContestants case
		if (line == "showContestants"){
			//print the line to the output file
			output << line << endl;
			
			//print a line for each player in the heap
			for(int i =0; i< numPlayers; i++){
				output << "Contestant <"<<players[i].id<<"> in extended heap location <"<<i+1<<"> with score <"<<players[i].points<<">." << endl;
			}
			
		}
		
		//findContestant case
		if (line == "findContestant"){
			//grab the parameter and print the line to output file
			input >> arg1;
			output << line << " " << arg1 << endl;
			arg1 = arg1.substr(1, arg1.size()-2);
			argi1=stoi(arg1);
			
			//if the contestant is not in handle array print error message
			if(handles[argi1]==-1){
				output << "Contestant <"<<argi1<<"> is not in the extended heap." << endl;
			}
			
			//otherwise print the found player to the output file
			else{
				output << "Contestant <"<<argi1<<"> is in the extended heap with score <"<<players[handles[argi1]].points<<">." << endl;
			}
		}
		
		//eliminateWeakest case
		if (line == "eliminateWeakest"){
			//print the line to output file
			output << line << endl;
			
			//if there's nobody in the heap, print an error message
			if (numPlayers ==0){
				output << "No contestant can be eliminated since the extended heap is empty." << endl;
			}
			
			//otherwise, remove the weakest, update the handle array, heapify if necessary, and print message to output file
			else{
				output << "Contestant <"<<players[0].id<<"> with current lowest score <"<<players[0].points<<"> eliminated." << endl;
				numPlayers--;
				swapHandle(&handles[players[0].id], &handles[players[numPlayers].id]);
				handles[1]=-1;
				players[0] = players[numPlayers];
				heapify(players,handles, numPlayers, 0);
			}
		}
		
		//earnPoints case
		if (line == "earnPoints"){
			
			//grab the two parameters and print line to the output file
			input >>arg1;
			input >> arg2;
			output << line << " " << arg1 << " " << arg2 << endl;
			arg1 = arg1.substr(1, arg1.size()-2);
			arg2 = arg2.substr(1, arg2.size()-2);
			argi1=stoi(arg1);
			argi2=stoi(arg2);
			
			//check if player is in the heap
			in =0;
			for (int i=0; i < numPlayers; i++){
				if(players[i].id == argi1){in =1;found = i;}
			}
			
			//if player is in the heap, increase their score, heapify if necessary, and output line to the output file
			if(in ==1){
				players[found].points+=argi2;
				output << "Contestant <"<<argi1<<">’s score increased by <"<<argi2<<"> points to <"<<players[found].points<<">." << endl;
				heapify(players,handles, numPlayers,found);
			}
			
			//otherwise, print error message to output file
			else{
				output << "Contestant <"<<argi1<<"> is not in the extended heap." << endl;
			}
			
		}
		
		//losePoints case
		if (line == "losePoints"){
			//grab the two parameters and print line to output file
			input >>arg1;
			input >> arg2;
			output << line << " " << arg1 << " " << arg2 << endl;
			arg1 = arg1.substr(1, arg1.size()-2);
			arg2 = arg2.substr(1, arg2.size()-2);
			argi1=stoi(arg1);
			argi2=stoi(arg2);
			
			//check if the player is in the heap
			in =0;
			for (int i=0; i < numPlayers; i++){
				if(players[i].id == argi1){in =1;found = i;}
			}
			
			//if player is in the heap, decrease their score, heapify if necessary, and print message to output file
			if(in ==1){
				players[found].points-=argi2;
				output << "Contestant <"<<argi1<<">’s score decreased by <"<<argi2<<"> points to <"<<players[found].points<<">." << endl;
				while(found !=0 && players[(found-1)/2].points > players[found].points){
					swap(&players[found], &players[(found-1)/2]);
					swapHandle(&handles[players[found].id], &handles[players[(found-1)/2].id]);
					found = (found-1)/2;
				}
			}
			
			//otherwise, print error message to output file
			else{
				output << "Contestant <"<<argi1<<"> is not in the extended heap." << endl;
			}
		}
		
		//showHandles case
		if (line == "showHandles"){
			//print line to output file
			output << line << endl;
			
			//print the message for each entry in the handles array. Print an error message if that entry is -1 
			for (int i=1; i < totalPlayers+1; i++){
				if(handles[i]==-1){
					output << "There is no Contestant <"<<i<<"> in the extended heap: handle[<"<<i<<">] = -1." << endl;
				}
				else{
					output << "Contestant <"<<i<<"> stored in extended heap location <"<<handles[i]+1<<">." << endl;
				}
			}
		}
		
		//showLocation case
		if (line == "showLocation"){
			//grab the parameter and output the line to the output file
			input >> arg1;
			output << line << " " << arg1 << endl;
			arg1 = arg1.substr(1, arg1.size()-2);
			argi1=stoi(arg1);
			
			//If the handles array is empty, print error message
			if(handles[argi1]==-1){
				output << "There is no Contestant <"<<argi1<<"> in the extended heap: handle[<"<<argi1<<">] = -1." << endl;
			}
			//otherwise, print location of of player in the handles array
			else{
				output << "Contestant <" << argi1 << "> stored in extended heap location <" << handles[argi1]+1<<">." << endl;
			}
		}
		
		//crownWinner case
		if (line == "crownWinner"){
			//print the message to the output file
			output << line << endl;
			//keep removing the weakest player until there's only one player left, update handles array and heapify when necessary
			while(numPlayers!=1){
				numPlayers--;
				swapHandle(&handles[players[0].id], &handles[players[numPlayers].id]);
				handles[1]=-1;
				players[0] = players[numPlayers];
				heapify(players,handles, numPlayers, 0);
			}
			//output the winner of the competition to the output file
			output << "Contestant <"<<players[0].id<<"> wins with score <"<<players[0].points<<">!"<<endl;
		}
	
		//changer for the while loop
		input >> line;
	}

}