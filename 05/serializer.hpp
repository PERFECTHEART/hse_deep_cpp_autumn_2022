#pragma once
#include <iostream>
#include <sstream>
#include <cstdint>

// Варианты исхода
enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {}

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class T, class... ArgsT>
    Error operator()(T&& val, ArgsT&&... args)
    {
        process(val);
        return (*this)(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error operator()(T&& val)
    {
        return process(val);
    }

    Error process(bool &arg)
    {
#ifdef DEBUG0
        std::cout << "Serializing bool: " << arg << std::endl;
#endif
        std::string val;
        if (arg)
        {
            val = "true";
        }
        else
        {
            val = "false";
        }
#ifdef DEBUG0
        std::cout << "Serialized bool: " << val << std::endl;
#endif
        if (out_.tellp() == std::streampos(0))
        {
            out_ << val;
        }
        else 
        {
            out_ << Separator;
            out_ << val;
        }
        return Error::NoError;
    }

    Error process(uint64_t &arg)
    {
#ifdef DEBUG0
        std::cout << "Serializing int: " << arg << std::endl;
#endif
        if (out_.tellp() == std::streampos(0))
        {
            out_ << arg;
        }
        else 
        {
            out_ << Separator;
            out_ << arg;
        }
        return Error::NoError;
    }

    
private:
    std::ostream& out_;
};

class Deserializer
{
    static constexpr char Separator = ' ';
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {}


    template <class T>
    Error load(T& object)
    {
        return object.deserialize(*this);
    }

    template <class T, class... ArgsT>
    Error operator()(T&& val, ArgsT&&... args)
    {
        Error err = load(val);
        if (err != Error::NoError)
        {
            return Error::CorruptedArchive;
        }
        return (*this)(std::forward<ArgsT>(args)...);
    }

    template <class T>
    Error operator()(T&& val)
    {
        return load(val);
    }

    Error load(bool& value)
    {
        std::string text;
        in_ >> text;
#ifdef DEBUG0
        std::cout << "Serializing bool: " << text << std::endl;
#endif

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
        {
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    Error load(uint64_t& value)
    {
        std::string text;
        in_ >> text;
#ifdef DEBUG0
        std::cout << "Deserializing int: " << text << std::endl;
#endif
        if (isNumeric(text))
        {
            std::istringstream iss(text);
            iss >> value;
        }
        else
        {
	    #ifdef DEBUG0
	    std::cout << "Invalid value" << std::endl;
            #endif
            return Error::CorruptedArchive;
        }
        return Error::NoError;
    }

    bool isNumeric(std::string const &str)
    {
        auto it = str.begin();
        while (it != str.end() && std::isdigit(*it)) {
            it++;
        }
        return !str.empty() && it == str.end();
    }
    
private:
    std::istream& in_;
};

struct Data
{
    uint64_t a;
    bool b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};

// Структура для передачи неверного формата
struct Data3i
{
    uint64_t a;
    uint64_t b;
    uint64_t c;

    template <class Serializer>
    Error serialize(Serializer& serializer)
    {
        return serializer(a, b, c);
    }

    template <class Deserializer>
    Error deserialize(Deserializer& deserializer)
    {
        return deserializer(a, b, c);
    }
};
