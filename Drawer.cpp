#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <ctime>
using namespace std;
bool check_in_list(string name, list <string> list); // Prototype for checking if name resides in given list
bool check_in_rules(string name, vector <vector <string> > rulebook); // Prototype for checking the existence of name in rulebook
bool check_in_drawn(string name, vector <vector <string> > drawnlist); // Prototype for checking if a name has already been drawn
int main() {
	char pause;
	cout << " ****** Welcome to Secret Santa Drawer v.1.0 ******" << endl << endl;
	cout << "Please enter the number of participants: ";
	int numberofPeople;
	cin >> numberofPeople; 
	cout << endl << endl;
	list <string> nameList; // Create sring list with initial size of number of participants
	cin.ignore(10000, '\n');
	string str; // placeholder variable for string, used for swapping values - NEVER DISPLAYED
	for (int i = 0; i < numberofPeople; i++) {
		cout << "Please enter the name of participant number " << i + 1 << ": ";
		getline(cin, str);
		nameList.push_back(str);
	}
	int numberofGifts;
	cout << "Please enter the number of gifts each participant is providing: ";
	cin >> numberofGifts;
	cin.ignore(10000, '\n');
	char ruleChoice;
	cout << "Are there any specific rules regarding who can get who? (Y/N): ";
	cin >> ruleChoice;
	cin.ignore(10000, '\n');
	ruleChoice = toupper(ruleChoice);
	vector <vector <string> > ruleMatrix;
	string nameforRuleA;
	string nameforRuleB;
	int nameNumforRuleA;
	int nameNumforRuleB;
	while (ruleChoice == 'Y') {
		switch (ruleChoice) {
		case 'Y':
			cout << "Please specify the name of the person to whom the rule pertains (caps sensitive): ";
			getline(cin, nameforRuleA);
			while (!(check_in_list(nameforRuleA, nameList))) {
				cout << "That name was not found in the list." << endl;
				cout << "Please specify the name of the person to whom the rule pertains (caps sensitive): ";
				getline(cin, nameforRuleA);
			}
			cout << "Please specify the name of the person who cannot be drawn (caps sensitive): ";
			getline(cin, nameforRuleB);
			while (!(check_in_list(nameforRuleB, nameList))) {
				cout << "That name was not found in the list." << endl;
				cout << "Please specify the name of the person who cannot be drawn (caps sensitive): ";
				getline(cin, nameforRuleB);
			}
			ruleMatrix.push_back({ nameforRuleA, nameforRuleB });
			ruleMatrix.push_back({ nameforRuleB, nameforRuleA }); // accounting for inverse, A not B therefore B not A
			cout << "Do you wish to add another rule? (Y/N): ";
			cin >> ruleChoice;
			ruleChoice = toupper(ruleChoice);
			cin.ignore();
			break;
		case 'N':
			break;
		}
	}
	cout << endl << "****** Rules ****** (disregard if none)" << endl;
	for (int i = 0; i < ruleMatrix.size(); i++) {
		cout << ruleMatrix[i][0] << " cannot draw " << ruleMatrix[i][1] << endl;
	}
	cout << endl << "Confirm? (Y/N): ";
	cin >> ruleChoice;
	ruleChoice = toupper(ruleChoice);
	if (ruleChoice == 'N') {
		cout << "Oops! Please try again. (restart program)" << endl;
		cin >> pause;
	}
	list <string> pickingHat = nameList; // virtual "hat" of drawable names
	/* for (std::list <string>::iterator ItZ = pickingHat.begin(); ItZ != pickingHat.end(); ++ItZ) {
		cout << "PICKABLE NAMES: " << *ItZ << endl;
	} */     // FOR DEBUGGING
	cout << endl;
	vector <vector <string> > drawingList; // final list of drawn names
	int pickedSlot; // placeholder variable for position of picked name on list
	std::list <string>::iterator ItB; // placeholder variable for drawn name
	vector <string> withheldNames; // placeholder vector for temporarily withheld names due to rules
	/* Because rules exist, for small groups, the people with exceptions must go first to avoid no-valid-draw scenario */
	/* This section will identify all exception names and move them to front of list */
	for (std::list <string>::iterator ItC = nameList.begin(); ItC != nameList.end(); ++ItC) {
		if (check_in_rules(*ItC, ruleMatrix)) {
			withheldNames.push_back(*ItC);
		}
	}
	for (int i = 0; i < withheldNames.size(); i++) {
		nameList.remove(withheldNames[i]);
		nameList.push_front(withheldNames[i]);
	} 
	/* Begin the drawing process, ensuring every draw is valid through procedural elimination, avoiding redraws */
	/* Do/While loop for ensuring correct amount of re-runs corresponding to number of gifts per participant */
	int z = 0;
	srand((unsigned int)time(NULL)); // seeding RNG with time preceding drawing
	do {
		pickingHat = nameList;
		std::cout << "Gift Round Number " << (z + 1) << endl << endl;
		for (std::list <string>::iterator ItA = nameList.begin(); ItA != nameList.end(); ++ItA) {
			pickingHat.remove(*ItA); // the drawer cannot draw their own name
			/* Check if any rules exist for the given name, if so, remove name(s) pertaining to rules */
			if (check_in_rules(*ItA, ruleMatrix)) {
				for (int i = 0; i < ruleMatrix.size(); i++) {
					if (ruleMatrix[i][0] == *ItA) {
						pickingHat.remove(ruleMatrix[i][1]);
						withheldNames.push_back(ruleMatrix[i][1]);
						// cout << "NAME REMOVED" << endl;     // DEBUGGING LINE
					}
				}
			}
			/* for (std::list <string>::iterator ItZ = pickingHat.begin(); ItZ != pickingHat.end(); ++ItZ) {
				cout << "PICKABLE NAMES: " << *ItZ << endl;
			} */      // FOR DEBUGGING
			pickedSlot = rand() % (pickingHat.size());
			ItB = pickingHat.begin();
			for (int i = 0; i < pickedSlot; i++)
				++ItB;
			drawingList.push_back({ *ItA, *ItB });
			// cout << "DRAWN: " << *ItA << " is buying a gift for " << *ItB << endl;   // DEBUGGING LINE
			pickingHat.remove(*ItB); // once a name is drawn, it cannot be drawn again
			if (!(check_in_drawn(*ItA, drawingList)))
				pickingHat.push_back(*ItA); // returns the name of the drawer back into the pool ONLY if if the drawner hasn't been drawn yet
			for (int i = 0; i < withheldNames.size(); i++) // loop which restores the temporarily withheld names due to rules ONLY IF NOT DRAWN
			{
				if (!(check_in_drawn(withheldNames[i],drawingList)))
				pickingHat.push_back(withheldNames[i]);
				// cout << "WITHHELD RULE NAME RESTORED" << endl;     // DEBUGGING LINE
			}
			withheldNames.clear();
		}
		/* Print the final list of drawn names for the respective round (multiple rounds if multiple gifts per participant) */
		for (int i = 0; i < drawingList.size(); i++) {
			std::cout << drawingList[i][0] << " is buying a gift for " << drawingList[i][1] << endl;
		}
		cout << endl << endl;
		z++;
		withheldNames.clear();
		drawingList.clear();
		pickingHat.clear();
		pickedSlot = 0;
	} while (z < numberofGifts);

	
	cin >> pause;
}
/* Function for checking the existence of name in given list */
bool check_in_list(string name, list <string> list) {
	bool inList = false;
	for (std::list <string>::iterator It = list.begin(); It != list.end(); It++) {
		if (*It == name)
			inList = true;
		else continue;
	}
	return inList;
}
/* Function for checking the existence of name in rulebook */
bool check_in_rules(string name, vector <vector <string> > rulebook) {
	bool inRules = false;
	for (int i = 0; i < rulebook.size(); i++) {
		for (int j = 0; j < 2; j++) {
			if (rulebook[i][j] == name)
				inRules = true;
		}
	}
	return inRules;
}
/* Function for checking if a name has already been drawn */
bool check_in_drawn(string name, vector <vector <string> > drawnlist) {
	bool inDrawn = false;
	for (int i = 0; i < drawnlist.size(); i++) {
		if (name == drawnlist[i][1])
			inDrawn = true;
	}
	return inDrawn;
}