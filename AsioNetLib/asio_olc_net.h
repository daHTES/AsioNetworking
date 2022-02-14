#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>


#include "asio_net_message.h"
#include "asio_net_lib.h"
#include "asio_net_client.h"
#include "asio_net_message.h"
#include "asio_net_connection.h"
#include "asio_net_server.h"
#include "asio_net_tsqueue.h"

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