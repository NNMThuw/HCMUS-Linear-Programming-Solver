#include<iostream>
#include<string>
#include <iomanip>
#include<sstream> 
#include <algorithm> 

using namespace std;


struct problem {
	int numberVariable;
	int numberConstraint;
	//Ham muc tieu
	double* z;
	bool isMin;
	//Cac rang buoc
	double** A;
	double* b;
	//Dau cua rang buoc
	int* signInequalityConstraint;
	//Dau cua rang buoc dau
	int* signVariableConstraint;
	int numberNewVariable
};

void rotatePivot(double** Table, int numberRow, int numberCol, int xPivot, int yPivot);
int convertInequalitySignToInt(string str) {
	if (str == "<=") return -1;
	if (str == "=") return 0;
	return 1;
}
string convertIntToInequalitySign(int sign) {
	if (sign == -1) return "<=";
	if (sign == 0) return "=";
	return ">=";
}

void inputProblem(problem* P) {
	cout << endl << setfill('-') << setw(30) << "-" << " NHAP BAI TOAN " << setw(30) << "-" << setfill(' ') << endl << endl;
	P->numberNewVariable = 0;
	cout << "   Nhap so luong bien: ";
	cin >> P->numberVariable;
	cout << "   Nhap so rang buoc (khong tinh rang buoc ve dau cua cac bien): ";
	cin >> P->numberConstraint;
	//Ham muc tieu
	string min;
	cout << "\n   Neu tim MIN, vui long nhap \"Min\" nguoc lai (tim MAX) nhap \"Max\": ";
	cin >> min;
	transform(min.begin(), min.end(), min.begin(), std::tolower);
	if (min == "min") {
		P->isMin = true;
	}
	else {
		P->isMin = false;
	}
	string tempS;
	P->z = new double[P->numberVariable * 2];
	P->signVariableConstraint = new int[P->numberVariable * 2];
	cout << "\n       HAM MUC TIEU\n";
	cout << "   Nhap he so ham muc tieu tuong ung voi (x1, x2, ..., xn), cac he so cach nhau boi <khoang trang>:\n";
	cout << "   Vi du: z = 3x1 -2x2; Nhap: 3 -2\n";
	cout << "   Nhap: ";
	for (int i = 0; i < P->numberVariable; i++) {
		cin >> P->z[i];
	}

	//Rang buoc
	P->A = new double* [P->numberConstraint * 2];
	P->b = new double[P->numberConstraint * 2];
	P->signInequalityConstraint = new int[P->numberConstraint * 2];
	for (int j = 0; j < P->numberConstraint; j++)
		P->A[j] = new double[P->numberVariable * 2 + P->numberConstraint];
	cout << "\n       RANG BUOC (BAT) DANG THUC";
	for (int j = 0; j < P->numberConstraint; j++) {
		cout << "\n   Nhap cac he so tuong ung voi (x1, x2, ..., xn) cua RB" << j + 1 << ": ";
		for (int i = 0; i < P->numberVariable; i++) {
			cin >> P->A[j][i];
		}
		cout << "   Nhap dau cua rang buoc (>=, =, <=): ";
		cin >> tempS;
		P->signInequalityConstraint[j] = convertInequalitySignToInt(tempS);
		cout << "   Nhap b" << j + 1 << ": ";
		cin >> P->b[j];
	}

	//Rang buoc dau
	cout << "\n       RANG BUOC VE DAU\n";
	cout << "   Nhap lan luot cac rang buoc ve dau cua cac bien (x1, x2, ..., xn).\n";
	cout << "   Nhap 1 neu bien >= 0, -1 neu bien <= 0, 0 neu bien tu do:\n";
	cout << "   Vi du: x1 tu do, x2 <= 0; Nhap: 0 -1\n";
	cout << "   Nhap: ";
	for (int i = 0; i < P->numberVariable; i++) {
		cin >> P->signVariableConstraint[i];
	}
	cout << endl << setfill('-') << setw(75) << "-" << setfill(' ') << endl << endl;

}

void deleteProblem(problem* P) {
	delete[] P->z;
	for (int i = 0; i < P->numberConstraint; i++) {
		delete P->A[i];
	}
	delete[] P->A;
	delete[] P->b;
	delete[] P->signInequalityConstraint;
	delete[] P->signVariableConstraint;
}


void printProblem(problem* P) {
	cout << endl << setfill('-') << setw(int(8 * (P->numberVariable + 2) / 2)) << "-" << " PROBLEM " << setw(int(8 * (P->numberVariable + 2) / 2)) << "-" << setfill(' ') << endl << endl;
	//In ham muc tieu
	if (P->isMin)
		cout << left << setw(8) << "Min z = ";
	else
		cout << left << setw(8) << "Max z = ";
	for (int i = 0; i < P->numberVariable; i++) {
		string temp = "";
		if (i > 0 && P->z[i] >= 0) temp += "+";
		if (int(P->z[i]) == P->z[i]) {
			temp += to_string(int(P->z[i])) + "*x" + to_string(i + 1);
		}
		else {
			stringstream s;
			s.precision(4);
			s << P->z[i];
			temp += s.str() + "*x" + to_string(i + 1);
		}
		cout << right << setw(9) << temp;
	}
	cout << endl;

	//In rang buoc
	for (int j = 0; j < P->numberConstraint; j++) {
		if (P->A[j][0] < 0) {
			cout << setw(8) << "";
		}
		else {
			cout << setw(8) << "";
		}
		for (int i = 0; i < P->numberVariable; i++) {
			string temp = "";
			if (i > 0 && P->A[j][i] >= 0) temp += "+";
			if (int(P->A[j][i]) == P->A[j][i]) {
				temp += to_string(int(P->A[j][i])) + "*x" + to_string(i + 1);
			}
			else {
				stringstream s;
				s.precision(4);
				s << P->A[j][i];
				temp += s.str() + "*x" + to_string(i + 1);
			}
			cout << right << setw(9) << temp;
		}
		string temp = " " + convertIntToInequalitySign(P->signInequalityConstraint[j]);
		cout << right << setw(4) << temp << right << setw(6) << setprecision(4) << P->b[j] << setw(2) << "(" << j + 1 << ")," << setprecision(6) << endl;
	}

	//In rang buoc dau
	for (int i = 0; i < P->numberVariable; i++) {
		if (P->signVariableConstraint[i] == 1)
			cout << setw(15) << "" << "x" << i + 1 << right << setw(9 * P->numberVariable+1) << ">=     0" << endl;
		else if (P->signVariableConstraint[i] == -1)
			cout << setw(15) << "" << "x" << i + 1 << right << setw(9 * P->numberVariable+1) << "<=     0" << endl;
	}
	cout << endl << setfill('-') << setw(8 * (P->numberVariable + 3) + 1) << "-" << setfill(' ') << endl << endl;

}
void printTable(double** Table, int numberRow, int numberCol) {
	cout << "\nTable:\n";
	for (int i = 0; i < numberRow; i++) {
		for (int j = 0; j < numberCol; j++) {
			cout << right << setw(7) << setprecision(4) << Table[i][j] << setprecision(6);
		} cout << endl;
	}
}


void convertIntoNormalForm(problem* P) {
	// Ham muc tieu
	if (!P->isMin) {
		for (int i = 0; i < P->numberVariable; i++) {
			P->z[i] = -P->z[i];
		}
	}

	// Rang buoc ve dau
	for (int i = 0; i < P->numberVariable - P->numberNewVariable; i++) {
		if (P->signVariableConstraint[i] == -1) { // xi <= 0
			// Ham muc tieu
			P->z[i] = -P->z[i];

			// Rang buoc dang thuc
			for (int j = 0; j < P->numberConstraint; j++) {
				P->A[j][i] = -P->A[j][i];
			}
		}
		else if (P->signVariableConstraint[i] == 0) { // xi tuy y => Add column xi- 
			P->numberVariable += 1;
			P->numberNewVariable += 1;
			// Ham muc tieu
			P->z[P->numberVariable - 1] = -P->z[i];

			// Rang buoc dang thuc
			for (int j = 0; j < P->numberConstraint; j++) {
				P->A[j][P->numberVariable - 1] = -P->A[j][i];
			}

		}
	}

	// Rang buoc dang thuc
	for (int i = 0; i < P->numberConstraint; i++) {
		if (P->signInequalityConstraint[i] == 1) { // >= bi
			for (int j = 0; j < P->numberVariable; j++) {
				P->A[i][j] = -P->A[i][j];
			}
			P->b[i] = -P->b[i];
			P->signInequalityConstraint[i] = -1; // Gan lai dau cho RB 
		}
		else if (P->signInequalityConstraint[i] == 0) { // = bi => Add new constraint
			P->numberConstraint += 1;
			P->A[P->numberConstraint - 1] = new double[P->numberVariable * 2];
			for (int j = 0; j < P->numberVariable; j++) {
				P->A[P->numberConstraint - 1][j] = -P->A[i][j];
			}
			P->b[P->numberConstraint - 1] = -P->b[i];

			P->signInequalityConstraint[i] = -1;
			P->signInequalityConstraint[P->numberConstraint - 1] = -1;
		}
	}
}
void converIntoTableForm(problem* P, double** Table) {
	// Dua ham muc tieu vao bang
	for (int j = 0; j < P->numberVariable; j++) {
		Table[0][j] = P->z[j];
	}

	// Dua cac rang buoc dang thuc vao bang
	for (int i = 1; i < 1 + P->numberConstraint; i++) {
		// Doi voi cac bien ban dau (Bien khong co so) Xj
		for (int j = 0; j < P->numberVariable; j++) {
			Table[i][j] = P->A[i - 1][j];
		}
		// Doi voi cac bien co so  Wj
		for (int j = P->numberVariable; j < P->numberVariable + P->numberConstraint; j++) {
			if (i - 1 == j - P->numberVariable) {
				Table[i][j] = 1;
			}
		}
		// 
		Table[i][P->numberVariable + P->numberConstraint] = P->b[i - 1];
	}


}


int chooseAlgorithm(problem const* PStand) {
	bool bland = false;
	for (int i = 0; i < PStand->numberConstraint; i++) {
		if (PStand->b[i] < 0) // 2 Pha
			return 2;

		if (PStand->b[i] == 0) // Bland
			bland = 1;
	}
	return bland; // Don hinh
}


int findArgMinRatio(double** table, int numberRow, int numberCol, int yPivot, bool phase1) {
	int i = 0;
	int xPivot = -1;
	double minRatio = -1;
	double ratio = 0;
	for (i = 1; i < numberRow; i++) {
		if (table[i][yPivot] > 0) {
			minRatio = table[i][numberCol - 1] / table[i][yPivot];
			xPivot = i;
			break;
		}
	}
	if (xPivot == -1) //Co bien vao nhung khong co bien ra -> bai toan khong gioi noi
		return -1;
	for (i = i + 1; i < numberRow; i++) {
		if (table[i][yPivot] > 0) {
			ratio = table[i][numberCol - 1] / table[i][yPivot];
			if (ratio < minRatio) {
				minRatio = ratio;
				xPivot = i;
			}
			if (phase1) {
				if (ratio == minRatio && table[i][numberCol - 2] == 1) {
					xPivot = i;
				}
			}
		}
	}
	return xPivot;
}
int choosePivotDantzig(double** table, int numberRow, int numberCol, int& xPivot, int& yPivot, bool phase1) { //Pivot = Table[x][y]
	double minC = 0;
	int i;
	yPivot = -1;
	for (i = 0; i < numberCol - 1; i++) {
		if (table[0][i] < 0 && table[0][i] < minC) {
			minC = table[0][i];
			yPivot = i;
		}
	}
	if (yPivot == -1) //Ham muc tieu da thoa dieu kien dung
		return 0;
	//Bien vao la bien o cot yPivot
	xPivot = findArgMinRatio(table, numberRow, numberCol, yPivot, phase1);
	if (xPivot == -1)
		return -1; //Co bien vao nhung khong co bien ra -> infity
	return 1;
}
int choosePivotBland(double** table, int numberRow, int numberCol, int& xPivot, int& yPivot) {
	int i = 0;
	yPivot = -1;
	for (; i < numberCol - 1; i++) {
		if (table[0][i] < 0) {
			yPivot = i;
			break;
		}
	}
	if (yPivot == -1) //Da thoa dieu kien dung
		return 0;
	xPivot = findArgMinRatio(table, numberRow, numberCol, yPivot, 0);
	if (xPivot == -1)
		return -1; //Co bien vao nhung khong co bien ra -> infity
	return 1;
}
void rotatePivot(double** Table, int numberRow, int numberCol, int xPivot, int yPivot) {
	for (int i = 0; i < numberRow; i++) {
		if (i != xPivot) {
			double coef = -Table[i][yPivot] / Table[xPivot][yPivot];
			//cout << coef << " " << i << " : \n";
			for (int j = 0; j < numberCol; j++) {
				Table[i][j] += coef * Table[xPivot][j];
			}
		}
		else {
			double coef = Table[xPivot][yPivot];
			for (int j = 0; j < numberCol; j++) {
				Table[xPivot][j] /= coef;
			}
		}
	}
}
int findPivotOfColumn(double** const table, int numberRow, int numberCol, int col) {
	int xPivot = -1;
	bool flag = false;
	for (int i = 1; i < numberRow; i++) {
		if (table[i][col] == 0)
			continue;
		if (table[i][col] == 1) {
			if (flag == false) {
				xPivot = i; //xPivot la so 1 dau tien va duy nhat
				flag = true;
			}
			else
				return -1; //Ko co pivot o cot nay
		}
		else
			return -1;
	}
	return xPivot;
}


int dantzig(double** table, int numberRow, int numberCol, bool phase1 = 0) {
	int xPivot = -1, yPivot = -1;
	while (true) {
		int check = choosePivotDantzig(table, numberRow, numberCol, xPivot, yPivot, phase1);
		if (check == 1) {
			rotatePivot(table, numberRow, numberCol, xPivot, yPivot);
		}
		else {				//check = -1: co bien vao khong co bien ra, 0: dung
			return -check;	//1: khong gioi noi, 0: dung
		}
	}
	return 0;
}
int bland(double** table, int numberRow, int numberCol) {
	int xPivot = -1, yPivot = -1;
	while (true) {
		int check = choosePivotBland(table, numberRow, numberCol, xPivot, yPivot);
		if (check != 1) {	//check = -1: co bien vao khong co bien ra, 0: dung
			return -check;	//1: khong gioi noi, 0: dung
		}
		else {
			rotatePivot(table, numberRow, numberCol, xPivot, yPivot);
		}
	}
}
int twoPhase(double** table, int numberRow, int numberCol) {
	//Lap bang moi
	double** tableP1 = new double* [numberRow];
	tableP1[0] = new double[numberCol + 1]{ 0 };
	tableP1[0][numberCol - 1] = 1;
	tableP1[0][numberCol] = table[0][numberCol - 1];
	for (int i = 1; i < numberRow; i++) {
		tableP1[i] = new double[numberCol + 1];
		for (int j = 0; j < numberCol - 1; j++) {
			tableP1[i][j] = table[i][j];
		}
		tableP1[i][numberCol - 1] = -1;
		tableP1[i][numberCol] = table[i][numberCol - 1];
	}
	int xPivot = -1, yPivot = numberCol - 1;
	double minB = 0;
	for (int i = 1; i < numberRow; i++) {
		if (table[i][yPivot] < minB) {
			minB = table[i][yPivot];
			xPivot = i;
		}
	}
	rotatePivot(tableP1, numberRow, numberCol + 1, xPivot, yPivot);
	dantzig(tableP1, numberRow, numberCol + 1, 1);
	//Da dat "tu vung" toi uu
	for (int j = 0; j < numberCol - 1; j++) {
		if (tableP1[0][j] != 0)
			return -1; //Bai toan vo nghiem
	}

	//Chuyen sang Pha 2
	for (int i = 1; i < numberRow; i++) {
		for (int j = 0; j < numberCol - 1; j++) {
			table[i][j] = tableP1[i][j];
		}
		table[i][numberCol - 1] = tableP1[i][numberCol];
	}
	for (int j = 0; j < numberCol; j++) {
		xPivot = findPivotOfColumn(table, numberRow, numberCol, j);
		if (xPivot == -1)
			continue;
		rotatePivot(table, numberRow, numberCol, xPivot, j);
	}
	//Da thu duoc tu vung moi
	//Ap dung Dantzig de giai
	int check = dantzig(table, numberRow, numberCol);
	for (int i = 1; i < numberRow; i++) {
		delete[] tableP1[i];
	}
	delete[] tableP1;
	return check;
}


bool checkOneRoot(problem const* P, double** Table, int numberCol, int* pivots) {
	for (int i = 0; i < numberCol - 1; i++) {
		if (i >= P->numberVariable - P->numberNewVariable && i < P->numberVariable)
			continue;
		if (pivots[i] == -1 && abs(Table[0][i]) < 1e-4 && P->signVariableConstraint[i] != 0)
			return false;
	}
	return true;
}
int findNameVariable(problem* P, double** Table, int numberCol, int index, string& name) {
	name = "";
	if (index < P->numberVariable - P->numberNewVariable) {
		name = "x" + to_string(index + 1);
		return 1;
	}
	else if (index + 1 > P->numberVariable && index + 1 < numberCol) {
		name = "w" + to_string(index + 1 - P->numberVariable);
		return 1;
	}
	return 0;
}

void outputOfTable(problem* P, double** Table, int numberRow, int numberCol, int result) {
	cout << endl << setfill('-') << setw(15) << "-" << " KET QUA BAI TOAN " << setw(15) << "-" << setfill(' ') << endl << endl;
	if (result == 1) { // Khong gioi noi
		if (P->isMin) {
			cout << "   Bai toan Khong gioi noi. \n   Tuc la Min z = -" << static_cast<unsigned char>(236) << endl;
		}
		else {
			cout << "   Bai toan Khong gioi noi. \n   Tuc la Max z = +" << static_cast<unsigned char>(236) << endl;
		}
		return;
	}
	else if (result == 0) { // Bai toan dat dieu kien dung: Nghiem duy nhat hoac VSN
		// In GTTU
		if (P->isMin) {
			cout << "   Min z = " << -Table[0][numberCol - 1] << endl << endl;
		}
		else {
			cout << "   Max z = " << Table[0][numberCol - 1] << endl << endl;
		}
		// In nghiem toi uu
		int* pivots = new int[numberCol - 1]{ 0 };
		for (int i = 0; i < numberCol - 1; i++) {
			pivots[i] = findPivotOfColumn(Table, numberRow, numberCol, i);
		}
		if (checkOneRoot(P, Table, numberCol, pivots)) { // Nghiem duy nhat
			cout << "   Nghiem toi uu la: \n\n";
			for (int j = 0; j < P->numberVariable - P->numberNewVariable; j++) {
				if (Table[0][j] != 0) {
					cout << "   x" << j + 1 << " = " << right << setw(6) << setprecision(4) << 0 << setprecision(6) << endl;
					continue;
				}
				double count = 0;
				int index = 0;
				for (int i = 1; i < numberRow; i++) {
					if (Table[i][j] != 0)
					{
						count++;
						index = i;
					}
				}
				double temp = round(Table[index][numberCol - 1] / Table[index][j] * 1e4) / 1e4;
				if (P->signVariableConstraint[j] == -1)
					cout << "   x" << j + 1 << " = " << right << setw(6) << setprecision(4) << -temp << setprecision(6) << endl;
				else
					cout << "   x" << j + 1 << " = " << right << setw(6) << setprecision(4) << temp << setprecision(6) << endl;
			}
		}
		else { // VSN
			cout << "   Bai toan co vo so nghiem.\n";
			cout << "   Nghiem toi uu co dang: \n\n";

			int* sign = new int[numberCol - 1];
			for (int i = 0; i < numberCol - 1; i++) {
				if (i < P->numberVariable - P->numberNewVariable && P->signVariableConstraint[i] < 0)
					sign[i] = -1;
				else
					sign[i] = 1;
			}
			string nameVar = "";

			for (int i = 0; i < P->numberVariable - P->numberNewVariable; i++) {
				if (pivots[i] == -1) {
					if (abs(Table[0][i]) > 1e-4) {
						cout << "   x" << i + 1 << " = 0" << endl;
					}
					else {
						if (P->signVariableConstraint[i] == 0) {
							cout << "   x" << i + 1 << " tu do" << endl;
						}
						else {
							cout << "   x" << i + 1 << " " << convertIntToInequalitySign(P->signVariableConstraint[i]) << " 0 " << endl;
						}
					}
				}
				else {
					cout << "   x" << i + 1 << "  = ";
					cout << sign[i] * Table[pivots[i]][numberCol - 1] << " ";
					for (int j = 0; j < numberCol - 1; j++) {
						if (abs(Table[0][j]) > 1e-4 || pivots[j] != -1 || j == i) {
							continue;
						}
						if (findNameVariable(P, Table, numberCol, j, nameVar) == 0) //Bien moi do bien tu do sinh ra thi khong can xuat
							continue;
						else {
							if (-sign[i] * sign[j] * Table[pivots[i]][j] == 0)
								continue;
							if (-sign[i] * sign[j] * Table[pivots[i]][j] > 0)
								cout << "+ ";
							cout << -sign[i] * sign[j] * Table[pivots[i]][j] << nameVar << " ";
						}
					}
					cout << endl;
				}
			}
			cout << "\n   Thoa (cac) dieu kien:\n\n";
			for (int i = 0; i < numberCol - 1; i++) {
				if (i >= P->numberVariable - P->numberNewVariable && i < P->numberVariable)
					continue;
				if (i < P->numberVariable - P->numberNewVariable && P->signVariableConstraint[i] == 0)
					continue;
				if (pivots[i] == -1) {
					if (i < P->numberVariable - P->numberNewVariable)
						continue;
					if (abs(Table[0][i]) < 1e-4) {
						if (findNameVariable(P, Table, numberCol, i, nameVar) == 1) {
							cout << "   ";
							if (i >= P->numberVariable - P->numberNewVariable) {
								cout << nameVar << " >= 0 \n";
							}
							else {
								if (P->signVariableConstraint[i] == 0) {
									cout << nameVar << " tu do" << endl;
								}
								else {
									cout << nameVar << " " << convertIntToInequalitySign(P->signVariableConstraint[i]) << " 0 " << endl;
								}
							}
						}
					}
				}
				else {
					cout << "   " << sign[i] * Table[pivots[i]][numberCol - 1];
					for (int j = 0; j < numberCol - 1; j++) {
						if (abs(Table[0][j]) > 1e-4 || pivots[j] != -1) {
							continue;
						} //Neu la bien phu thuoc hoac bien nam tren ham muc tieu (=0) khi ko xuat
						if (findNameVariable(P, Table, numberCol, j, nameVar) == 0) //Bien moi do bien tu do sinh ra thi khong can xuat
							continue;
						else {
							if (-sign[i] * sign[j] * Table[pivots[i]][j] == 0)
								continue;
							if (-sign[i] * sign[j] * Table[pivots[i]][j] > 0)
								cout << "+ ";
							cout << -sign[i] * sign[j] * Table[pivots[i]][j] << nameVar << " ";
						}
					}

					cout << " >= 0 \n";
				}
			}
			delete[] sign;
		}
		delete[] pivots;
	}
	else {
		cout << "   Bai toan vo nghiem!\n";
	}
	cout << endl << setfill('-') << setw(48) << "-" << setfill(' ') << endl << endl;
}
void solveProblem(problem* P) {
	// Dua P ve dang chuan
	convertIntoNormalForm(P);

	// Dua P o dang chuan thanh bang
	double** Table = new double* [P->numberConstraint * 2];
	for (int i = 0; i < P->numberConstraint * 2; i++) {
		Table[i] = new double[P->numberVariable * 2 + P->numberConstraint * 2]{ 0 };
	}
	int numberRow = P->numberConstraint + 1;
	int numberCol = P->numberConstraint + P->numberVariable + 1;
	converIntoTableForm(P, Table);
	// Chon thuat toan de xoay
	int xPivot = -1,
		yPivot = -1;
	int check;
	if (chooseAlgorithm(P) == 0) // Don hinh
	{
		check = dantzig(Table, numberRow, numberCol);
	}
	else if (chooseAlgorithm(P) == 1) // Bland
	{
		check = bland(Table, numberRow, numberCol);
	}
	else // 2 Pha
	{
		check = twoPhase(Table, numberRow, numberCol);
	}
	outputOfTable(P, Table, numberRow, numberCol, check);

	for (int i = 0; i < P->numberConstraint * 2; i++) {
		delete[] Table[i];
	}
	delete[] Table;
}


int main() {

	problem P;

	inputProblem(&P);
	printProblem(&P);
	solveProblem(&P);
	deleteProblem(&P);
	return 0;
}