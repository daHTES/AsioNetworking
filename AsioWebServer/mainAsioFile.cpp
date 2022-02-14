#include <iostream>
#include <chrono>
#include <thread>
#ifdef _WIN32
#define _WIN32_WINT 0x0A00
#endif

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


std::vector<char> mAsioBuffer(20*1024);

void GetSomeTempData(asio::ip::tcp::socket &socket) 
{
	socket.async_read_some(asio::buffer(mAsioBuffer.data(), mAsioBuffer.size()),
		[&](std::error_code ec, std::size_t lenght)
		{
			if (!ec)
			{
				std::cout << "\nRead " << lenght << std::endl;
				for (int i = 0; i < lenght; i++)
					std::cout << mAsioBuffer[i];

				GetSomeTempData(socket);
			}
		}
	);
}

int main() 
{
	setlocale(LC_ALL, "ru");


	// обработчик ошибок
	asio::error_code mAsioErrorCode;
	// контект он же IU
	asio::io_context mAsioContext;
	// Завглушка для контекста
	asio::io_context::work mIdleWorkContext(mAsioContext);
	// Поток для контекста
	std::thread mAsiothreadContext = std::thread([&]() {mAsioContext.run(); });
	// адресс подключения
	asio::ip::tcp::endpoint mAsioEndPointConnect
	(asio::ip::make_address("212.42.76.253", mAsioErrorCode), 80);
	// делаю сокет
	asio::ip::tcp::socket mAsioSocket(mAsioContext);
	// пробую подключение
	mAsioSocket.connect(mAsioEndPointConnect, mAsioErrorCode);


	// проверка на случай, если объект error выкинет ошибку
	if (!mAsioErrorCode) 
	{
		std::cout << "Подлючение прошло!" << std::endl;
	}
	else 
	{
		std::cout << "Подключение не прошло по адрессу: \n" << mAsioErrorCode.message() << std::endl;
	}

	if (mAsioSocket.is_open()) 
	{
		GetSomeTempData(mAsioSocket);

		std::string REST =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";
			

		mAsioSocket.write_some(asio::buffer(REST.data(), REST.size()), mAsioErrorCode);


		using namespace std::chrono_literals;
		std::this_thread::sleep_for(20000ms);

		mAsioContext.stop();
		if (mAsiothreadContext.joinable())
			mAsiothreadContext.join();

	}


	system("pause");
	return 0;
}