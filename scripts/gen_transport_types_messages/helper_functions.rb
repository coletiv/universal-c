require 'yaml'

def load_types(types_yml_file_path)
  types = begin
    YAML.load(File.open(types_yml_file_path))
  rescue ArgumentError => e
    puts "Could not parse YAML: #{e.message}"
  end
  types
end

# helper functions

def string_c_struct_s_with_type_name(type_name)
  "struct transport_type_"+type_name+"_s"
end

def string_c_struct_t_with_type_name(type_name)
  "transport_type_"+type_name+"_t"
end

def string_c_type_for_property_type(property_type)
  case property_type
  when "string"
    "const char *"
  when "integer"
    "int64_t *"
  when "unsigned integer"
    "uint64_t *"
  end
end

def generate_c_function_arguments_for_properties(properties)
  c_function_arguments = ""
  properties.each do |property_name, property_type|
    if c_function_arguments.size >  0
      c_function_arguments += ", "
    end
    c_type_string = string_c_type_for_property_type property_type
    c_function_arguments += c_type_string+" "+property_name
  end
  c_function_arguments
end

def generate_c_function_parameters_for_properties_with_prefix(properties, prefix)
  c_function_parameters = ""
  properties.each do |property_name, property_type|
    if c_function_parameters.size >  0
      c_function_parameters += ", "
    end
    c_function_parameters += prefix+property_name
  end
  c_function_parameters
end

def generate_c_line_with(strings)
  c_line = ""
  strings.each do |string|
    c_line += string
  end
  c_line
end

class Array
  def push_c_line_with(*strings)
    self.push(generate_c_line_with(strings))
  end
end

def generate_c_file_with(generated_lines_array, prefix_source_file_path, suffix_source_file_path, generated_source_file_path)
  prefix_source_file = File.open(prefix_source_file_path, 'rb') { |file| file.read }
  suffic_source_file = File.open(suffix_source_file_path, 'rb') { |file| file.read }

  File.open(generated_source_file_path, 'w') do |c_file| 
    c_file.write(prefix_source_file)
    generated_lines_array.each do |generated_line|
      c_file.write generated_line+"\n"
    end
    c_file.write(suffic_source_file)
  end
end

def string_c_property_assign_create(object_name, property_name, property_type)
  case property_type
  when "string"
    generate_c_line_with([object_name,"->",property_name," = ", property_name, " != NULL ? ", "strdup(",property_name,") : NULL;"])
  when "integer"
    generate_c_line_with(["if(",property_name," != NULL){",object_name,"->",property_name," = (int64_t*)malloc(sizeof(int64_t));","memcpy(",object_name,"->",property_name,",",property_name,",sizeof(int64_t));} else {",object_name,"->",property_name," = NULL;}"])
  when "unsigned integer"
    generate_c_line_with(["if(",property_name," != NULL){",object_name,"->",property_name," = (uint64_t*)malloc(sizeof(uint64_t));","memcpy(",object_name,"->",property_name,",",property_name,",sizeof(uint64_t));} else {",object_name,"->",property_name," = NULL;}"])
  end
end

def string_c_property_release(object_name, property_name, property_type)
  # All properties are dynamically allocated, so they are freed the same way
  generate_c_line_with(["if(",object_name,"->",property_name,"){","free((void *)",object_name,"->",property_name,");","};"])
end

def string_c_property_log(object_name, property_name, property_type)
  case property_type
  when "string"
    generate_c_line_with(["Logc(\"  %s\",",object_name,"->",property_name,");"])
  when "integer"
    generate_c_line_with(["Logc(\"  %d\", *(",object_name,"->",property_name,"));"])
  when "unsigned integer"
    generate_c_line_with(["Logc(\"  %u\", *(",object_name,"->",property_name,"));"])
  end
end

def string_c_property_pack(msgpack_packer, value, property_name, property_type)
  string_c = ""
  case property_type
  when "string"
    string_c += generate_c_line_with(["msgpack_pack_str(",msgpack_packer,",strlen(\"",property_name,"\"));"])
    string_c += generate_c_line_with(["msgpack_pack_str_body(",msgpack_packer,",\"",property_name,"\",strlen(\"",property_name,"\"));"])
    string_c += generate_c_line_with(["if(",value," != NULL){"])
    string_c += generate_c_line_with(["msgpack_pack_str(",msgpack_packer,",strlen(",value,"));"])
    string_c += generate_c_line_with(["msgpack_pack_str_body(",msgpack_packer,",",value,",strlen(",value,"));"])
    string_c += generate_c_line_with(["} else {"])
    string_c += generate_c_line_with(["msgpack_pack_nil(",msgpack_packer,");"])
    string_c += generate_c_line_with(["}"])
  when "integer"
    string_c += generate_c_line_with(["msgpack_pack_str(",msgpack_packer,",strlen(\"",property_name,"\"));"])
    string_c += generate_c_line_with(["msgpack_pack_str_body(",msgpack_packer,",\"",property_name,"\",strlen(\"",property_name,"\"));"])
    string_c += generate_c_line_with(["if(",value," != NULL){"])
    string_c += generate_c_line_with(["msgpack_pack_int64(",msgpack_packer,",*",value,");"])
    string_c += generate_c_line_with(["} else {"])
    string_c += generate_c_line_with(["msgpack_pack_nil(",msgpack_packer,");"])
    string_c += generate_c_line_with(["}"])
  when "unsigned integer"
    string_c += generate_c_line_with(["msgpack_pack_str(",msgpack_packer,",strlen(\"",property_name,"\"));"])
    string_c += generate_c_line_with(["msgpack_pack_str_body(",msgpack_packer,",\"",property_name,"\",strlen(\"",property_name,"\"));"])
    string_c += generate_c_line_with(["if(",value," != NULL){"])
    string_c += generate_c_line_with(["msgpack_pack_uint64(",msgpack_packer,",*",value,");"])
    string_c += generate_c_line_with(["} else {"])
    string_c += generate_c_line_with(["msgpack_pack_nil(",msgpack_packer,");"])
    string_c += generate_c_line_with(["}"])
  end
  string_c
end

def string_c_property_unpack(msgpack_object, key, value, property_name, property_type)
  string_c = ""
  case property_type
  when "string"
    string_c += generate_c_line_with(["if (",msgpack_object,".type == MSGPACK_OBJECT_STR){"])
    string_c += generate_c_line_with(["if (strcmp(",key,",\"",property_name,"\") == 0){"])
    string_c += generate_c_line_with(["char * value = (char * )calloc(",msgpack_object,".via.str.size+1, sizeof(char));"])
    string_c += generate_c_line_with(["strncpy(value, ",msgpack_object,".via.str.ptr, ",msgpack_object,".via.str.size);"])
    string_c += generate_c_line_with([value," = (char * )calloc(strlen(value), sizeof(char));"])
    string_c += generate_c_line_with(["strcpy((char*)",value,", value);"])
    string_c += generate_c_line_with(["free((void*)value);"])
    string_c += generate_c_line_with(["}"])
    string_c += generate_c_line_with(["}"])
  when "integer"
    # TODO
  when "unsigned integer"
    # TODO
  end
  string_c
end
