#pragma once

#include "Globals.h"

struct json_array_t;
typedef json_array_t JSON_Array;

struct json_value_t;
typedef json_value_t JSON_Value;

struct json_object_t;
typedef json_object_t JSON_Object;

class GnJSONArray;

class GnJSONObj 
{
public:
	GnJSONObj();
	GnJSONObj(const char* buffer);
	GnJSONObj(JSON_Object* object);
	~GnJSONObj();

	JSON_Object* GetJSONObject();
	JSON_Value* GetValue();

	uint Save(char** buffer);

	JSON_Array* GetParsonArray(const char* name);

	int GetInt(const char* name);
	float GetFloat(const char* name);
	bool GetBool(const char* name);
	const char* GetString(const char* name);

	void AddInt(const char* name, int number);
	void AddFloat(const char* name, float number);
	void AddBool(const char* name, bool boolean);
	void AddString(const char* name, const char* string);
	GnJSONArray AddArray(const char* name, bool nested = false);
	GnJSONArray AddArray(GnJSONArray array);

private:
	JSON_Object* _object;
	JSON_Object* _root_object;
	JSON_Value* _root;
};

class GnJSONArray 
{
public:
	GnJSONArray();
	GnJSONArray(JSON_Array* array);
	GnJSONArray(JSON_Array* array, JSON_Object* root_object);
	GnJSONArray(const char* name);
	~GnJSONArray();

	GnJSONObj GetObjectInArray(const char* name);
	JSON_Object* GetRoot();

	void AddObject(GnJSONObj object);
	void AddFloat(float number);
	void AddString(const char* string);

private: 
	JSON_Array* _array;
	JSON_Object* _root;
	bool _nested;
};
