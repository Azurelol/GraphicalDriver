#pragma once
#include "Include.h"

namespace Oxana
{
	struct EditableVariable
	{
		std::string label;
		EditableVariable(const std::string& label) : label(label) {}
	};
	
	struct EditableFloat : EditableVariable
	{
		float* value;
		float min;
		float max;
		bool slider;
		EditableFloat(const std::string& label, float* value, float min, float max, bool slider) : EditableVariable(label), value(value), min(min), max(max), slider(slider) {}
	};

	struct EditableInt: EditableVariable
	{
		int* value;
		int min;
		int max;
		bool slider;
		EditableInt(const std::string& label, int* value, int min, int max, bool slider) : EditableVariable(label), value(value), min(min), max(max), slider(slider) {}
	};

	struct EditableBool : EditableVariable
	{
		bool* value;
		EditableBool(const std::string& label, bool* value) : EditableVariable(label), value(value) {}
	};

}