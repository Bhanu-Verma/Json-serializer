#ifndef JSON_VALUE_H
#define JSON_VALUE_H

#include <unordered_map>
#include <variant>
#include <vector>
#include <string>
#include <memory>
#include <utility>

namespace json {
    enum JsonType { Null, Bool, Number, String, Array, Object };

    class Json;

    class JsonValue {
    public:
        using VariantType = std::variant<
            std::monostate,             // Null
            bool,                       // Bool
            double,                     // Number
            std::string,                // String
            std::vector<JsonValue>,     // Array
            std::unique_ptr<Json>       // Object
        >;

    private:
        JsonType type_;
        VariantType data_;

    public:
        // 1. Constructors
        JsonValue() : type_{Null}, data_{std::monostate{}} {}

        explicit constexpr JsonValue(std::nullptr_t null) 
            : type_{Null}, data_{std::monostate{}} {}

        explicit constexpr JsonValue(bool val) 
            : type_{Bool}, data_{val} {}

        explicit constexpr JsonValue(double val) 
            : type_{Number}, data_{val} {}

        explicit JsonValue(const std::string& val);
        explicit JsonValue(const char* val);
        explicit JsonValue(const std::vector<JsonValue>& val);
        
        explicit JsonValue(const Json& val);

        // 2. Special Member Functions 
        JsonValue(const JsonValue& other);
        JsonValue& operator=( const JsonValue& other );
        ~JsonValue();
        JsonValue(JsonValue&& other) noexcept;
        JsonValue& operator=(JsonValue&& other) noexcept;
        

        // 3. Getters
        VariantType& getVar() { return data_; }
        const VariantType& getVar() const { return data_; }
        JsonType type() const { return type_; }

        template<typename T>
        T& get() {
            return std::get<T>(data_);
        }

        template<typename T>
        const T& get() const {
            return std::get<T>(data_);
        }


        // 4. Assignment Operator
        template <typename T, 
        typename = std::enable_if_t<!std::is_same_v<std::__remove_cvref_t<T>, JsonValue>>>
        JsonValue& operator=(T&& val){
            using Type = std::__remove_cvref_t<T>;
            static_assert(
                std::is_same_v<Type, std::nullptr_t> ||
                std::is_same_v<Type, bool> ||
                std::is_same_v<Type, double> ||
                std::is_same_v<Type, std::string> ||
                std::is_same_v<Type, char*> ||
                std::is_same_v<Type, std::vector<JsonValue>> ||
                std::is_same_v<Type, Json>
            );

            JsonValue temp { std::forward<T>(val) };
            std::swap(type_, temp.type_);
            std::swap(data_, temp.data_);
            return *this;
        }


        // 5. Special subscript operator for nested object
        JsonValue& operator[]( const std::string& key );
    };
}

#endif