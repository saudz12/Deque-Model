#include "Deque.h"

int main() {
	
	Deque<> myDeq;

	int o = 0;
	int k = 0;
	int p = 0;

	while (true) {
		system("cls");
		std::cout << "1. Insert element\n2. Delete element\n3. Get element\n4. Print list\n";
		std::cin >> o;
		switch (o)
		{
		case 1: {
			system("cls");
			std::cout << "What to insert: ";
			std::cin >> k;
			std::cout << "\nHow:\n1. Push front\n2. Push back\n3. Insert at poz\n";
			std::cin >> o;
			switch (o)
			{
			case 1: {
				myDeq.push_front(k);
				std::cout << k << " was inserted!\n";
				break;
			}
			case 2:{
				myDeq.push_back(k);
				std::cout << k << " was inserted!\n";
				break;
			}
			case 3: {
				std::cout << "\nwhere: "; std::cin >> p;
				myDeq.insert(p, k);
				std::cout << k << " was inserted!\n";
				break;
			}
			default:
				break;
			}
			system("pause");
			break;
		}
		case 2: {
			system("cls");
			std::cout << "where:\n";
			std::cout << "\n1. Pop front\n2. Pop back\n3. Delete poz\n";
			std::cin >> o;
			switch (o)
			{
			case 1: {
				myDeq.pop_front();
				std::cout << "first element was deleted!\n";
				break;
			}
			case 2: {
				myDeq.pop_back();
				std::cout << "last element was deleted!\n";
				break;
			}
			case 3: {
				std::cout << "\nwhere: "; std::cin >> p;
				myDeq.delete_elem(p);
				std::cout << "element on poz " << p << " was deleted!\n";
				break;
			}
			default:
				break;
			}
			system("pause");
			break;
		}
		case 3: {
			system("cls");
			std::cout << "which: ";
			std::cin >> p;
			std::cout << myDeq[p] << '\n';
			system("pause");
			break;
		}
		case 4: {
			system("cls");
			myDeq.print();
			system("pause");
			break;
		}
		default:
			break;
		}
	}

	return 0;
}