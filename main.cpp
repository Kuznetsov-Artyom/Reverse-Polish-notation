#include <iostream>
#include "Calculator.hpp"
#include <limits>









int main()
{
	system("chcp 1251 > nul");

	while (true)
	{
		std::string str;

		std::cout << "input = ";
		std::getline(std::cin, str);

		try
		{
			Calculator testing(str);
			std::cout << testing.GetPolStr() << '\n';
			std::cout << testing.GetResult() << '\n';
		}
		catch (const ExceptionRecord& ex)
		{
			std::cout << "Code error: " << ex.GetCodeError() << '\n';
			std::cout << ex.what() << '\n';
			continue;
		}
	}

	/*Calculator test2("13.3 - 0.3");
	std::cout << test2.GetSrcStr() << '\n';
	std::cout << test2.GetPolStr() << '\n';
	test2.ShowTableVar();
	std::cout << "Result = " << test2.GetResult() << '\n';*/


	/*Calculator test("5 + (5) -7+abc/3.0");
	std::cout << test.GetSrcStr() << '\n';
	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << "Result = " << test.GetResult() << '\n';


	test = "0.5 ^ (0.25 + 0.932 ^ 2.0) / (0.3 + 20) ^ xD";
	test.SetValVar("xD", 3);
	test.Calculation();
	std::cout << test.GetSrcStr() << '\n';
	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << "Result = " << test.GetResult() << '\n';

	std::cout << '\n';

	test = "(4 + 59.6)^(1 / 3) * 0.005 + 3.14^opa";
	test.SetValVar("opa", 3);
	test.Calculation();
	std::cout << test.GetSrcStr() << '\n';
	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << "Result = " << test.GetResult() << '\n';

	std::cout << '\n';

	test = "25^0.5 + (27^(1/3))^2.6";
	std::cout << test.GetSrcStr() << '\n';
	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << "Result = " << test.GetResult() << '\n';*/

	


	return 0;
}