#include "json/json_value.hpp"
#include "json/json.hpp"

#include <cassert>

namespace json {
    JsonValue::JsonValue( std::string val ) 
        : type_{String}, data_{ std::move(val) } {}

    JsonValue::JsonValue(const char* val) 
        : type_{String}, data_{std::string(val)} {}

    JsonValue::JsonValue( std::vector<JsonValue> val) 
        : type_{Array}, data_{ std::move(val) } {}

    JsonValue::JsonValue( Json val ) 
        : type_{Object}, data_{std::make_unique<Json>( std::move(val) )} {}

    // Copy constructor
    JsonValue::JsonValue(const JsonValue& other) : type_{other.type_} {
        auto doCopy = [](const auto& val) -> decltype(data_) {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, std::unique_ptr<Json>>) {
                // Now compiler knows Json's copy constructor exists
                return val ? std::make_unique<Json>(*val) : nullptr;
            } else {
                return val;
            }
        };

        data_ = std::visit(doCopy, other.data_);
    }

    // Copy Assignment Operator
    JsonValue& JsonValue::operator=(const JsonValue &other)
    {
        auto doCopy = [](const auto& val) -> decltype(data_) {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, std::unique_ptr<Json>>) {
                // Now compiler knows Json's copy constructor exists
                return val ? std::make_unique<Json>(*val) : nullptr;
            } else {
                return val;
            }
        };

        type_ = other.type_;
        data_ = std::visit(doCopy, other.data_);
        return (*this);
    }

    // Necessary for unique_ptr with incomplete types in header
    JsonValue::~JsonValue() = default;
    JsonValue::JsonValue(JsonValue &&other) noexcept = default;
    JsonValue &JsonValue::operator=(JsonValue &&other) noexcept = default;

    JsonValue& JsonValue::operator[](const std::string &key)
    {
        if ( type_ == Null ) {
            data_ = std::make_unique<Json>();
            type_ = Object;
        }
        assert(type_ == Object);
        auto& obj = *(std::get<std::unique_ptr<Json>>(data_));
        return obj[key];
    }
}