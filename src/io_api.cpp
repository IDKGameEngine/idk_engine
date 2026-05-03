#include "idk/io_api.hpp"
#include "idk/io_service.hpp"


idk::IoApi::IoApi(idk::IoService *srv)
:   srv_(srv),
    writer_(srv->getRequestWriter())
{

}
