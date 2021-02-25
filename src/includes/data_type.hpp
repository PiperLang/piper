#pragma once

namespace piper {
    enum MetaType {
        MT_UNKNOWN,
        MT_CUSTOM,
        MT_U64,
        MT_I64
    };

    struct DataType {
        MetaType type;
        std::string extra_data;
    };
}
