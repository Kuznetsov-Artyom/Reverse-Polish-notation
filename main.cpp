#include <iostream>
#include "Arithmetic.hpp"
#include <limits>









int main()
{
	while (true)
	{
		std::string str;

		std::cout << "input = ";
		std::getline(std::cin, str);

		try
		{
			Arithmetic testing(str);
			std::cout << testing.GetSrcStr() << '\n';
			std::cout << testing.GetPolStr() << '\n';
			testing.ShowTableVar();
			std::cout << "Result = " << testing.GetResult() << '\n';
		}
		catch (const ExceptionRecord& ex)
		{
			std::cout << "Code error: " << ex.GetCodeError() << '\n';
			std::cout << ex.what() << '\n';
			continue;
		}
	}

	return 0;
}