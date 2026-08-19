// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <string>
#include <vector>

// Stand-in for code protoc generates from cld3/src/feature_extractor.proto.
// CLD3 never serializes its messages, FMLParser fills these descriptors in.
// Repeated fields are plain vectors, elements are addressed right after adding.

namespace chrome_lang_id {

class Parameter final {
public:
	[[nodiscard]] const std::string &name() const {
		return _name;
	}
	void set_name(const std::string &value) {
		_name = value;
	}

	[[nodiscard]] const std::string &value() const {
		return _value;
	}
	void set_value(const std::string &value) {
		_value = value;
	}

private:
	std::string _name;
	std::string _value;

};

class FeatureFunctionDescriptor final {
public:
	[[nodiscard]] const std::string &type() const {
		return _type;
	}
	void set_type(const std::string &value) {
		_type = value;
	}

	[[nodiscard]] const std::string &name() const {
		return _name;
	}
	void set_name(const std::string &value) {
		_name = value;
	}

	[[nodiscard]] bool has_argument() const {
		return _hasArgument;
	}
	[[nodiscard]] int argument() const {
		return _argument;
	}
	void set_argument(int value) {
		_argument = value;
		_hasArgument = true;
	}

	[[nodiscard]] int parameter_size() const {
		return int(_parameters.size());
	}
	[[nodiscard]] const Parameter &parameter(int index) const {
		return _parameters[index];
	}
	Parameter *add_parameter() {
		return &_parameters.emplace_back();
	}

	[[nodiscard]] int feature_size() const {
		return int(_features.size());
	}
	[[nodiscard]] const FeatureFunctionDescriptor &feature(int index) const {
		return _features[index];
	}
	FeatureFunctionDescriptor *mutable_feature(int index) {
		return &_features[index];
	}
	FeatureFunctionDescriptor *add_feature() {
		return &_features.emplace_back();
	}

private:
	std::string _type;
	std::string _name;
	int _argument = 0;
	bool _hasArgument = false;
	std::vector<Parameter> _parameters;
	std::vector<FeatureFunctionDescriptor> _features;

};

class FeatureExtractorDescriptor final {
public:
	[[nodiscard]] int feature_size() const {
		return int(_features.size());
	}
	[[nodiscard]] const FeatureFunctionDescriptor &feature(int index) const {
		return _features[index];
	}
	FeatureFunctionDescriptor *mutable_feature(int index) {
		return &_features[index];
	}
	FeatureFunctionDescriptor *add_feature() {
		return &_features.emplace_back();
	}

private:
	std::vector<FeatureFunctionDescriptor> _features;

};

} // namespace chrome_lang_id
