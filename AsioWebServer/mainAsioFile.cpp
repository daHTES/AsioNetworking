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


	// ���������� ������
	asio::error_code mAsioErrorCode;
	// ������� �� �� IU
	asio::io_context mAsioContext;
	// ��������� ��� ���������
	asio::io_context::work mIdleWorkContext(mAsioContext);
	// ����� ��� ���������
	std::thread mAsiothreadContext = std::thread([&]() {mAsioContext.run(); });
	// ������ �����������
	asio::ip::tcp::endpoint mAsioEndPointConnect
	(asio::ip::make_address("212.42.76.253", mAsioErrorCode), 80);
	// ����� �����
	asio::ip::tcp::socket mAsioSocket(mAsioContext);
	// ������ �����������
	mAsioSocket.connect(mAsioEndPointConnect, mAsioErrorCode);


	// �������� �� ������, ���� ������ error ������� ������
	if (!mAsioErrorCode) 
	{
		std::cout << "���������� ������!" << std::endl;
	}
	else 
	{
		std::cout << "����������� �� ������ �� �������: \n" << mAsioErrorCode.message() << std::endl;
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