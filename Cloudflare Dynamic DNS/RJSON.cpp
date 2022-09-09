#include "RJSON.h"
namespace RJSON
{
	inline JSONElement RJSON::EmptyElem{ "", "" };
	
	JSONElement& JSONElement::get(std::string _name)
	{
		for (JSONElement& elem : children)
			if (elem.name == _name)
				return elem;
		return RJSON::EmptyElem;
	}
	JSONElement& JSONElement::get(std::string _name, int _level)
	{
		for (JSONElement& elem : children)
			if (elem.name == _name)
				return elem;

		if (_level != 0 && !children.empty())
			return get(_name, --_level);
		return RJSON::EmptyElem;
	}
	JSONElement& JSONElement::addChild(std::string _name, std::string _value, int _type)
	{
		JSONElement elem;
		elem.name = _name;
		elem.value = value;
		elem.type = _type;
		return addChild(elem);
	}
	JSONElement& JSONElement::addChild(JSONElement _jsonElement)
	{
		children.push_back(_jsonElement);
		return children.back();
	}
	void JSONElement::removeChild(std::string _name, size_t _index)
	{
		size_t count = 0;
		for (size_t i = 0; i < children.size(); i++)
			if (children[i].name == _name)
				if (count == _index)
				{
					children.erase(children.begin() + i);
				}
				else
					count++;
	}
	void JSONElement::removeChild(size_t _index)
	{
		children.erase(children.begin() + _index);
	}
	const unsigned int JSONElement::countChildren(std::string _name)
	{
		int count = 0;
		for (const JSONElement& elem : children)
			if (elem.name == _name)
				count++;
		return count;
	}
	const bool JSONElement::hasChild(std::string _name)
	{
		for (const JSONElement& elem : children)
			if (elem.name == _name)
				return true;
		return false;
	}
	const std::string JSONElement::rawString()
	{
		std::string jsonstructure;
		if (type == Array)
		{
			jsonstructure = "[";
			for (size_t i = 0; i < children.size(); i++)
				if (children[i].type == Object)
			            jsonstructure += "{" + children[i].rawString2(i == children.size() - 1 ? true : false) + "}";
				else if (children[i].type == Array)
			           jsonstructure += "[" + children[i].rawString2(i == children.size() - 1 ? true : false) + "]";
				else
				   jsonstructure += children[i].rawString2(i == children.size() - 1 ? true : false);
			jsonstructure += "]";
		}
		else {// Everything else
			jsonstructure = "{";
			for (size_t i = 0; i < children.size(); i++)
				jsonstructure += children[i].rawString2(i == children.size() - 1 ? true : false);
			jsonstructure += "}";
		}

		return jsonstructure;
	}
	const std::string JSONElement::rawString2(bool _islast)
	{
		std::string jsonstructure;
		if (name.empty())
		{// Parent is Array type

			jsonstructure += rawValue();
			if (!_islast)
				jsonstructure += ",";
			return jsonstructure;
		}
		jsonstructure = "\"" + name + "\": ";

		if (type == Object || type == Array)
		{
			jsonstructure += rawString();
		}
		else
		{
			jsonstructure += rawValue();
		}

		if (!_islast)
			jsonstructure += ",";
		return jsonstructure;
	}
	const std::string JSONElement::rawValue()
	{
		switch (type)
		{
		case String:
			return "\"" + value + "\"";
		case Integer:
			return value;
		case Float:
			return value;
		case Boolean:
			return value;
		case Null:
			return value;
		default:// Handle like string
			return "\"" + value + "\"";
		}
	}
	const int JSONElement::getType()
	{
		return type;
	}
	void JSONElement::autoType()
	{
		if (!children.empty())
		{
			if (children.front().name.empty())
			{
				type = Array;
			}
			else
				type = Object;
			return;
		}

		if (value.empty())
		{// The type could be anything 
			if (name.empty())
			{
				type = Unknown;
			}
			else
				type = String;
			return;
		}
		if (value.find_first_not_of("-1234567890.") == std::string::npos)// e and E are not included here | Eulers numbers not supported
		{// Number
			if (value.find('.') != std::string::npos)
			{// Float
				type = Float;
			}
			type = Integer;
			return;
		}
		if (value == "true" || value == "false")
		{// Boolean
			type = Null;
			return;
		}
		if (value == "null")
		{
			type = Null;
			return;
		}
		// String
		type = String;
		return;
	}
	const int JSONElement::valueAsInt()
	{
		return stoi(value);
	}
	const float JSONElement::valueAsFloat()
	{
		char* _Eptr;// that would be for error checking but im not gonna do that
		return strtof(value.c_str(), &_Eptr);
	}
	const std::string JSONElement::valueAsString()
	{
		return value;
	}
	const bool JSONElement::valueAsBool()
	{
		if (value[0] == 't')
			return true;
		return false;
	}

	const bool JSONElement::operator==(const JSONElement& _right)
	{
		return name == _right.name && value == _right.value;
	}


	// Public
	JSONElement RJSON::load(std::string _jsonstructure)
	{
		size_t pos = 0;
		return parse(_jsonstructure, pos);
	}

	// Private
	JSONElement RJSON::parseElement(const std::string& _data, size_t& _off, int _type) {
		JSONElement elem;

		if (_type == Array)
		{
			size_t end = 0;
			size_t pos = _data.find('"', _off);
			for (end = _data.find('"', ++pos); end != std::string::npos && _data[end - 1] == '\\'; end = _data.find('"', ++pos));
			if (size_t off = _data.find_first_not_of(JSONWhitespace, end + 1); end != std::string::npos && off != std::string::npos &&_data[off] == ',' || _data[off] == ']')
			{// array element is string
				_off = pos;
				elem.value = _data.substr(_off, end - _off);
				_off = end;
				return elem;
			}
		}

		size_t end = _data.find('"', ++_off);
		if (end == std::string::npos || _off == std::string::npos)
			return elem;

		elem.name = _data.substr(_off, end - _off);
		_off = ++end;
		_off = _data.find_first_of("\"{[-1234567890n", _off);
		if (_off ==std::string::npos)
			return elem;

		if (_data[_off] == '"')
		{// string
			// parsing the content
			for (end = _data.find('"', ++_off); _data[end - 1] == '\\'; end = _data.find('"', ++_off));
			elem.value = _data.substr(_off, end - _off);
			_off = end;
			elem.type = String;
			return elem;
		}
		else if (_data[_off] == '{')
		{// object
			elem.children = parse(_data, _off).children;
			elem.type = Object;
			return elem;
		}
		else if (_data[_off] == '[')
		{// array
			elem.children = parse(_data, _off).children;
			elem.type = Array;
			return elem;
		}
		else if (std::string str; &(str += _data[_off]) || str.find_first_of("-1234567890") != std::string::npos)
		{// number
			end = _data.find(',');
			elem.value = _data.substr(_off, end - _off);
			_off = end;
			if (elem.value.find('.') != std::string::npos)
			{// Float
				elem.type = Float;
			}
			else
				elem.type = Integer;
			return elem;
		}
		else if (_data[_off] == 'n')
		{// null
			end = _data.find(',');
			elem.value = _data.substr(_off, end - _off);
			_off = end;
			elem.type = Null;
			return elem;
		}
		throw "JSON is wrong formatted.";
		return elem;
	}
	
	JSONElement RJSON::parse(const std::string& _data, size_t& _off, std::string _name)
	{
		JSONElement elem;
		_off = _data.find_first_of("[{", _off);
		if (_off == std::string::npos)
			return elem;

		if (_data[_off] == '{')
		{
			elem.type = Object;
		}
		else if (_data[_off] == '[')
		{
			elem.type = Array;
		}
		

		for (; _off < _data.length(); _off++)
		{
			size_t tmp = _off;
			tmp = _data.find_first_not_of(JSONWhitespace, ++tmp);
			if (tmp != std::string::npos)
				_off = tmp;
			if (_data[_off] == '}' || _data[_off] == ']')
			{
				return elem;
			}
			_off = _data.find('"', _off);
			elem.children.push_back(parseElement(_data, _off, elem.type));
		}

		return elem;
	}
}
