#include "quick_logger.h"

int main() {

    Logger::instance().init();

    LOG_FUNC_CALL

    LOG_INFO("test...")

    int i = 10;
    LOG_DEBUG("i = ", i)

    int j = 20;
    LOG_ERROR("i = ", i, ", j = ", j)

    return 0;
}