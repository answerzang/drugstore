#pragma warning(disable:4996)
#include<iostream>
#include<cmath>
using namespace std;

double change[] = { -1.5,-1,-0.5,0,2,4,6 };
double teProfit = 0, myProfit = -1111, midProfit = -1111, midProfit1,midProfit2;
//��0�����ϰ����в�������ȥ��ҩƷ������

bool read(int argc, char** argv);//���ļ�
void print(char** argv);	//д�ļ�
void getProfit();			//����̸�����
void greedy();				//̰�����ɳ�ʼ��
double frand() { return 1.0 * rand() / RAND_MAX; }
void SA();					//�˻�
void adjust();				//�����˻��
void calculate();			//��������

int strategy[10][10][2], mystrategy[10][10][2], midstrategy[10][10][2], midstrategy1[10][10][2],midstrategy2[10][10][2];//̰�Ĺ����м����
int haveSold[10][3], midHaveSold[10][3], midHaveSold1[10][3];//��¼�����ı��,�����ǳ�ʼ���
void Init();
int dele[50][2];		//ɾ��0
int indexs1[30], misindex1[50];		//�м����
struct drug {
	int day;				//������
	double value;			//����
	double deltava;			//ԭ����¼ҩƷ�Ӽ��ã�����Ϊ���۲��������¼�Ӽۣ�����û�õ�
	int in;					//�Ƿ��ڲֿ�
	int away;				//�Ƿ��ӵ�
	int sold;				//�Ƿ�����
	int id;					//ҩƷ��ţ���ҩ���̻������򣬸ı�ҩƷλ��
} drugs[50];

struct dNode {
	drug* d;
	double value;
}D[10];
int main(int argc, char** argv) {
	int re = read(argc, argv);
	if (!re)
		return 1;
	Init();
	getProfit();
	greedy();
	SA();
	print(argv);
//	cout << endl << myProfit;
	return 0;
}

bool read(int argc, char** argv) {
	if (argc != 7) {
		cout << "Lack file";
		return 0;
	}

	int i = 1;
	while (i < 4) {
		string jud = argv[i];
		if (jud == "-m") {
			FILE* fp = fopen(argv[++i], "r");
			if (!fp)
				return 0;
			for (int n = 0; n < 50; n++) {
				fscanf(fp, "%lf\t%d\n", &drugs[n].value, &drugs[n].day);
			}
			fclose(fp);
			i++;
		}
		else if (jud == "-s") {
			FILE* fp1 = fopen(argv[++i], "r");
			if (!fp1)
				return 0;
			for (int k = 0; k < 10; k++) {
				for (int j = 0; j < 10; j++) {
					fscanf(fp1, "%d,%d\t", &strategy[k][j][0], &strategy[k][j][1]);
				}
			}
			fclose(fp1);
			i++;
		}
		//		else if (jud == "-d") {
		//			FILE* fp2 = fopen(argv[++i], "r");
		//			if (!fp2)
		//				return 0;
		//			/*int n;
		//			fscanf(fp2, "%d", &n);
		//			for (int i = 0; i < n; i++) {
		//				fscanf(fp2, "%d%d", &dele[i][0], &dele[i][1]);
		//			}*/
		//			int n = 0;
		//			while (fscanf(fp2, "%d%d", &dele[n][0], &dele[n++][1])!=EOF) {
		//				;
		//			}
		//			fclose(fp2);
		//			i++;
		//		}
	}
	return 1;
}

void print(char** argv)
{	
	string filePath = string(argv[4]);
	string fileName1 = filePath.substr(0, filePath.length() - 12) + "mystrategy.txt";
	string fileName2 = filePath.substr(0, filePath.length() - 12) + "mydelete.txt";
	string fileName3 = filePath.substr(0, filePath.length() - 12) + "result.txt";
	string fileName4 = filePath.substr(0, filePath.length() - 12) + "delete.txt";

	FILE* file1 = fopen(fileName1.c_str(), "w");
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++) {
			if(mystrategy[i][j][0]!=-1)
				fprintf(file1, "%d,%d\t", drugs[mystrategy[i][j][0]].id, mystrategy[i][j][1]);
			else
				fprintf(file1, "-1,6\t");
		}
		fprintf(file1, "\n");
	}
	fclose(file1);

	FILE* file2 = fopen(fileName2.c_str(), "w");

	for (int i = 0; i < 50; i++) {
		if (drugs[i].away == 1) {
			fprintf(file2, "0\t%d\n", drugs[i].id);
		}
	}
	fclose(file2);

	FILE* file3 = fopen(fileName3.c_str(), "w");
	fprintf(file3, "�̸�:%g\n�ҵ�:%g\n", teProfit, myProfit);
	fclose(file3);

	FILE* file4 = fopen(fileName4.c_str(), "w");
	int k = 0;
	while (dele[k][0] != -1)
	{
		fprintf(file4, "%d\t%d\n", dele[k][0], dele[k][1]);
		k++;
	}
	fclose(file4);
}

void Init() {
	for (int i = 0; i < 50; i++) {
		drugs[i].sold = 0;
		drugs[i].away = 0;
		drugs[i].in = 1;
		drugs[i].id = i;
	}
}

void getProfit() {
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < 2; j++) {
			dele[i][j] = -1;
		}
	}
	int indexx = 0;					//��¼�ӵ�ҩƷ������
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 50; j++) {
			drugs[j].in = 1;
		}
		for (int j = 0; j < 10; j++) {	//�������
			if (strategy[i][j][0] != -1) {
				D[j].value = drugs[strategy[i][j][0]].value + change[strategy[i][j][1]];
				D[j].d = &drugs[strategy[i][j][0]];
				drugs[strategy[i][j][0]].in = 0;
			}
			else {
				D[j].value = 100;
			}
		}
		//����������ֵ��������С�ұ������
		for (int j = 0; j < 9; j++) {
			for (int k = j + 1; k < 10; k++) {
				if (D[j].value > D[k].value) {
					swap(D[j], D[k]);
				}
				else if (D[j].value == D[k].value && D[j].value != 100 && D[j].d->day < D[k].d->day) {
					swap(D[j], D[k]);
				}
			}
		}
		for (int j = 0; j < 3; j++) {
			if (D[j].value != 100) {
				teProfit += (D[j].value - D[j].d->value);
				D[j].d->sold = 1;
			}
		}
		//������
		for (int j = 0; j < 50; j++) {
			if (drugs[j].sold == 0 && drugs[j].day == i && drugs[j].away == 0) {//day==i
				drugs[j].away = 1;
				teProfit -= drugs[j].value;
				dele[indexx][0] = i+1;
				dele[indexx++][1] = drugs[j].id;
			}
		}

		//�ֿ����ѣ�sold=0����in=1����away=0
		for (int j = 0; j < 50; j++) {
			if (drugs[j].away == 0 && drugs[j].in == 1 && drugs[j].sold == 0) {
				if (drugs[j].day - i <= 5)
					teProfit -= 1;
				else {
					teProfit -= 0.5;
				}
			}
		}
	}
	for (int j = 0; j < 50; j++) {
		if (drugs[j].sold == 0 && drugs[j].day == 10 && drugs[j].away == 0) {//day==i
			drugs[j].away = 1;
			teProfit -= drugs[j].value;
			dele[indexx][0] = 11;
			dele[indexx++][1] = drugs[j].id;
		}
	}
}

void greedy() {//Ϊ������ң���¼������λ�ã���ʼλ����id��¼,ֻ��Ҫ�ó�һ��10��3������

	for (int i = 0; i < 50; i++) {//����
		drugs[i].deltava = 6;
	}

	Init();
	for (int i = 0; i < 49; i++) {
		for (int j = i + 1; j < 50; j++) {
			if (drugs[j].day < drugs[i].day) {
				swap(drugs[i], drugs[j]);
			}
			else if (drugs[j].day == drugs[i].day && drugs[j].value > drugs[i].value) {
				swap(drugs[i], drugs[j]);
			}
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			haveSold[i][j] = -1;
			midHaveSold[i][j] = -1;
		}
	}
	for (int i = 0; i < 10; i++) {
		int index[3] = { -1,-1,-1 };
		int num = 0;
		for (int j = 0; j < 50; j++) {//ȷ��ÿ����������
			if (drugs[j].day - i - 1 >= 0 && drugs[j].sold == 0 && num<3) {
				index[0] = index[1];
				index[1] = index[2];
				index[2] = j;
				num++;
			}
		}
		//�������ֱ�ǳ���,����index[]��
		for (int j = 0; j < 3; j++) {
			if (index[j] != -1) {
				drugs[index[j]].sold = 1;
				haveSold[i][j] = index[j];		//̰�ĳ�ʼ��haveSold��midHaveSold
				midHaveSold[i][j] = index[j];
			}
		}
	}
	//�����Ҫ����ҩ,���ɣ���10�����Ϲ��ڵĶ��������ݶ�Ϊ��
	for (int i = 0; i < 50; i++) {
		if (drugs[i].day <= 10 && drugs[i].sold == 0) {
			drugs[i].away = 1;					//drugs[i]���ᱻ����
			/* 
			ʵ���ϲ������Ͻ�����Ϊ��ֻ����ǰ��������ҩƷ��۸��С�ģ���������С�ģ�������Ϊ���ݱ��������٣�����
			drugs[i]�ļ۸��������������ʱ����ͬ��ҩƷ����С�ģ��㹻С���������кܴ���ʶ�����С�ģ�
			���������ǰ������һ��ѭ��������haveSold�������ڷ���Ҫ�󣩣��ҵ��۸���С�ģ�Ȼ����drugs[i]�ļ۸�Ƚ�
			�����ҵ�������˵�������ľ��Ǽ۸���С�ģ�����û���޸�
			*/
			for (int j = 0; j < 10; j++) {
				for (int k = 0; k < 3; k++) {
					if (drugs[i].away && drugs[midHaveSold[j][k]].day <= drugs[i].day && drugs[midHaveSold[j][k]].value < drugs[i].value) {
						drugs[midHaveSold[j][k]].away = 1;
						midHaveSold[j][k] = i;
						haveSold[j][k] = i;
						drugs[i].away = 0;
					}
				}
			}
		}
	}
	int num = 0;
}

void calculate() {/* ����midHaveSold���ɲ���midstrategy,����������midProfit */
	midProfit = 0;
	for (int i = 0; i < 50; i++) {
		drugs[i].deltava = 6;
		drugs[i].sold = 0;
		if (drugs[i].away == 1)
			midProfit -= drugs[i].value;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			midstrategy2[i][j][0] = -1;
			midstrategy2[i][j][1] = 6;
		}
	}
	for (int i = 0; i < 10; i++) {//�ж��Ƿ�Ϸ�
		for (int j = 0; j < 3; j++) {
			if (midHaveSold[i][j] != -1)
				drugs[midHaveSold[i][j]].sold = 1;
		}
	}
	for (int j = 0; j < 50; j++) {
		if (drugs[j].away == 0 && drugs[j].sold == 0) {
			misindex1[j] = j;
		}
		else {
			misindex1[j] = -1;
		}
	}
	for (int i = 0; i < 10; i++) {//������ҩƷ
		for (int j = 0; j < 3; j++) {
			indexs1[i * 3 + j] = midHaveSold[i][j];
		}
	}

	//����ÿ���strategy��һ���촦��ÿ�춼�����������ļӼ۲���
	for (int i = 0; i < 10; i++) {//ȱ�ݣ����ܴ���ÿ���ܰڷŵ�ҩ���ϵ�ҩƷ��������3�ֵ����
		midstrategy2[i][0][0] = midHaveSold[i][0];
		midstrategy2[i][1][0] = midHaveSold[i][1];
		midstrategy2[i][2][0] = midHaveSold[i][2];
		midProfit2 = 0;		//�����Ӽ�ʱ��������ֵ
		double maxP = -100;	//��¼�����������ֵ
		int n = 0;	//��������mid����
		int mid[50];//��¼��������û��������û������ҩƷ
		int no = 0;	//��¼�������ҩ���ϵ�ҩƷ������
		for (int j = 3 * (i + 1); j < 30; j++) {
			if (indexs1[j] != -1)mid[n++] = indexs1[j];
		}
		for (int j = 0; j < 50; j++) {
			if (drugs[j].away == 0 && drugs[j].sold == 0) {
				mid[n++] = j;
			}
		}

		//������ʱ������,�����Թ�ѡ��
		for (int j = 0; j < n - 1; j++) {
			for (int k = j + 1; k < n; k++) {
				if (drugs[mid[j]].day > drugs[mid[k]].day) {
					swap(mid[j], mid[k]);
				}
			}
		}

		for (int d1 = 6; d1 > 0 && no < 8; d1--) {
			/*
			no��¼�˵���ŵ�ҩ���ϵ�ҩƷ������ĳ��Ҫ������ҩƷ�Ӽ۴�6��Ϊ4ʱ������-2��
			ֻ�м��ٵĲֿ����Ѷ���2ʱ������Ż���ߣ�����������Ҫ����ҩƷ���ϼܣ�
			Ҳ����֮ǰҩ���ϵ�ҩƷ�����Ϊ7
			*/
			for (int d2 = 6; d2 > 0 && no < 8; d2--) {
				for (int d3 = 6; d3 > 0 && no < 8; d3--) {
					double maxD = ((drugs[midHaveSold[i][0]].value + change[d1]) > (drugs[midHaveSold[i][1]].value + change[d2])) ? (drugs[midHaveSold[i][0]].value + change[d1]) : (drugs[midHaveSold[i][1]].value + change[d2]);
					maxD = (maxD > (drugs[midHaveSold[i][2]].value + change[d3])) ? maxD : (drugs[midHaveSold[i][2]].value + change[d3]);
					midstrategy2[i][0][1] = d1;
					midstrategy2[i][1][1] = d2;
					midstrategy2[i][2][1] = d3;

					for (int k = 0; k < 50; k++) {
						drugs[k].in = 1;
					}
					midProfit2 = (change[d1] + change[d2] + change[d3]);
					for (int j = 0; j < 3; j++) {
						if (midHaveSold[i][j] != -1) {
							drugs[midHaveSold[i][j]].in = 0;
						}
					}
					no = 3;
					for (int j = 0; j < n; j++) {
						if (mid[j] != -1 && no<10 && (drugs[mid[j]].value + 6)>maxD) {//ȱ��
							midstrategy2[i][no][0] = mid[j];
							midstrategy2[i][no][1] = 6;
							no++;
							drugs[mid[j]].in = 0;
						}
					}
					for (int j = no; j < 10; j++) {
						midstrategy2[i][j][0] = -1;
						midstrategy2[i][j][1] = 6;
					}
					for (int j = 0; j < n; j++) {
						if (drugs[mid[j]].in == 1 && drugs[mid[j]].day - i <= 5) {
							midProfit2 -= 1;//�ֿ�����
						}
						else if (drugs[mid[j]].in == 1 && drugs[mid[j]].day - i > 5) {
							midProfit2 -= 0.5;
						}
					}
					if (midProfit2 >= maxP) {
						maxP = midProfit2;//�����м����۲�������
						for (int nn = 0; nn < 10; nn++) {
							midstrategy[i][nn][0] = midstrategy2[i][nn][0];
							midstrategy[i][nn][1] = midstrategy2[i][nn][1];
						}
					}
				}
			}
		}
		midProfit += maxP;
	}
}

void adjust() {
	for (int i = 0; i <= rand() % 5; i++) {
		while (1) {
			int opt = rand() % 4;
			if (opt == 0) {//������ҩƷ��δ������δ������ҩƷ����
				int x = rand() % 10, y = rand() % 3, z = rand() % 50;
				if (drugs[misindex1[z]].away == 0 && drugs[midHaveSold[x][y]].day >= 10 && drugs[misindex1[z]].day > x) {
					swap(midHaveSold[x][y], misindex1[z]);
					break;
				}
			}
			else {//������ҩƷ�໥����
				int x = rand() % 10, y = rand() % 3;
				int u = rand() % 10, v = rand() % 3;
				if (drugs[midHaveSold[x][y]].day > u && drugs[midHaveSold[u][v]].day > x) {//û����
					swap(midHaveSold[x][y], midHaveSold[u][v]);
					break;
				}
			}
		}
	}
}

void SA() {
	double T = 10000;
	calculate();
	myProfit = midProfit;
	midProfit1 = midProfit;
	cout << teProfit << endl;
	cout << myProfit << endl;
	memcpy(midHaveSold1, haveSold, sizeof(haveSold));
	while (T > 0.000001) {
		adjust();
		calculate();
		double delta = midProfit1 - midProfit;
		if (delta < 0 || exp(delta / T) < frand()) {
			midProfit1 = midProfit;
			memcpy(midHaveSold1, midHaveSold, sizeof(midHaveSold));
			if (myProfit - midProfit < 0) {
				memcpy(mystrategy, midstrategy, sizeof(midstrategy));
				myProfit = midProfit;
				memcpy(haveSold, midHaveSold, sizeof(midHaveSold));
			}
		}
		else {//�˻�midHaveSold���޸�
			memcpy(midHaveSold, midHaveSold1, sizeof(midHaveSold1));
		}
		T *= 0.999;//����6��������0.999���ˣ���6��0.9999
	}
	for (double i = 1; i <= 5000; i++) {
		/*��ɽ��ʵ���ϲ���ɽ����Ҳ����ı䣬�������˻�����㹻,������ﵽ�����Ž�*/
		adjust();
		calculate();
		if (midProfit > myProfit) {
			memcpy(mystrategy, midstrategy, sizeof(midstrategy));
			myProfit = midProfit;
			memcpy(haveSold, midHaveSold, sizeof(midHaveSold));
		}
	}
	//cout << endl;
	//for (int i = 0; i < 10; i++) {
	//	for (int j = 0; j < 10; j++) {
	//		cout << drugs[mystrategy[i][j][0]].id << ' ';
	//	}
	//	cout << endl;
	//}
	cout << myProfit << endl;
}
