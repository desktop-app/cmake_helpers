// This file is part of Desktop App Toolkit,
// a set of libraries for developing nice desktop applications.
//
// For license and copyright information please follow this link:
// https://github.com/desktop-app/legal/blob/master/LEGAL
//
#pragma once

#include <string>
#include <vector>

// Stand-in for code protoc generates from cld3/src/task_spec.proto.
// CLD3 never serializes its messages, TaskContext keeps settings in TaskSpec.
// Repeated fields are plain vectors, elements are addressed right after adding.

namespace chrome_lang_id {

class TaskInput final {
public:
	class Part final {
	public:
		[[nodiscard]] const std::string &file_pattern() const {
			return _filePattern;
		}
		void set_file_pattern(const std::string &value) {
			_filePattern = value;
		}

	private:
		std::string _filePattern;

	};

	[[nodiscard]] const std::string &name() const {
		return _name;
	}
	void set_name(const std::string &value) {
		_name = value;
	}

	[[nodiscard]] int file_format_size() const {
		return int(_fileFormats.size());
	}
	[[nodiscard]] const std::string &file_format(int index) const {
		return _fileFormats[index];
	}
	void add_file_format(const std::string &value) {
		_fileFormats.push_back(value);
	}

	[[nodiscard]] int record_format_size() const {
		return int(_recordFormats.size());
	}
	[[nodiscard]] const std::string &record_format(int index) const {
		return _recordFormats[index];
	}
	void add_record_format(const std::string &value) {
		_recordFormats.push_back(value);
	}

	[[nodiscard]] int part_size() const {
		return int(_parts.size());
	}
	[[nodiscard]] const Part &part(int index) const {
		return _parts[index];
	}
	Part *add_part() {
		return &_parts.emplace_back();
	}

private:
	std::string _name;
	std::vector<std::string> _fileFormats;
	std::vector<std::string> _recordFormats;
	std::vector<Part> _parts;

};

class TaskSpec final {
public:
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

	[[nodiscard]] int parameter_size() const {
		return int(_parameters.size());
	}
	[[nodiscard]] const Parameter &parameter(int index) const {
		return _parameters[index];
	}
	Parameter *mutable_parameter(int index) {
		return &_parameters[index];
	}
	Parameter *add_parameter() {
		return &_parameters.emplace_back();
	}

	[[nodiscard]] int input_size() const {
		return int(_inputs.size());
	}
	[[nodiscard]] const TaskInput &input(int index) const {
		return _inputs[index];
	}
	TaskInput *mutable_input(int index) {
		return &_inputs[index];
	}
	TaskInput *add_input() {
		return &_inputs.emplace_back();
	}

private:
	std::vector<Parameter> _parameters;
	std::vector<TaskInput> _inputs;

};

} // namespace chrome_lang_id
