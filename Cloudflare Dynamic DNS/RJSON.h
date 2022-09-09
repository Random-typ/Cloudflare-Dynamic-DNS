#pragma once
#include <string>
#include <vector>
namespace RJSON {
	constexpr const char* JSONWhitespace = " \t\n\r";
	struct JSONElement;
	
	enum JSONTypes : int
	{
		Unknown,
		Integer,
		Float,
		String,
		Boolean,
		Null,
		Object,
		Array
	};

	struct JSONElement
	{
	public:
		// get element by its name
		// if level < 0 global search will be performed
		JSONElement& get(std::string _name);
		JSONElement& get(std::string _name, int _level);


		JSONElement& addChild(std::string _name, std::string _value = "", int _type = Unknown);
		JSONElement& addChild(JSONElement _jsonElement);

		void                            removeChild(std::string _name, size_t _index = 0);
		void                            removeChild(size_t _index);

		const unsigned int              countChildren(std::string _name);

		const bool                      hasChild(std::string _name);

		const std::string               rawString();

		const int						getType();

		void							autoType();

		const int						valueAsInt();

		const float						valueAsFloat();

		const std::string				valueAsString();

		const bool						valueAsBool();

		const bool						operator==(const JSONElement& _right);

		std::string name;
		std::string value;
		std::vector<JSONElement> children;
		int type = Unknown;

	private:
		const std::string               rawString2(bool _islast);
		const std::string				rawValue();
	};
	

	class RJSON
	{
	public:
		static JSONElement EmptyElem;
		static JSONElement load(std::string _jsonstructure);

	private:
		static JSONElement parseElement(const std::string& _data, size_t& _off, int _type);
		static JSONElement parse(const std::string& _data, size_t& _off, std::string _name = "");
	};

	
}

