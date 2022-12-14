#include <iostream>
#include "Calculator.hpp"






int main()
{

	Calculator test("-1 * (-1) + 2 ^ (9) / 3");

	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	test = "5 + a + 5 -3*(-11) + aaa_1";

	std::cout << '\n';

	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	
	test.SetValVar("a", -23);
	test.Calculation();

	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	std::cout << '\n';

	test = "2+(-3) + abc ^ a";
	std::cout << test.GetResult() << "\n\n";
	test.SetValVar("abc", -5);
	test.SetValVar("a", 2);
	test.Calculation();
	std::cout << test.GetPolStr() << '\n';
	test.ShowTableVar();
	std::cout << test.GetResult() << '\n';

	std::cout << '\n';

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
	std::cout << "Result = " << test.GetResult() << '\n';

	


	return 0;
}