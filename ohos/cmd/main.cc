#include <stddef.h>
#include <iostream>

#include "ui/rs_node.h"
#include "ui/rs_surface_node.h"
#include "transaction/rs_transaction.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto node = OHOS::Rosen::RSSurfaceNode::Create({});
    std::cout << "node=" << node.get() << std::endl;
    OHOS::Rosen::RSTransaction::FlushImplicitTransaction();
    return 0;
}