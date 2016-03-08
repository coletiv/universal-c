load 'helper_functions.rb'

types = load_types(ARGV[0])

#############################
# universal_transport_types.h
#############################

universal_transport_types_header = []

# types enum 
universal_transport_types_header.push_c_line_with("enum {")
types.each_with_index do |type, type_index| 
  name = type["name"]
  universal_transport_types_header.push_c_line_with("UNIVERSAL_TRANSPORT_TYPE_",name.upcase," = ",(type_index+1).to_s,",")
end
universal_transport_types_header.push_c_line_with("};")

# types declaration
types.each do |type| 
  name = type["name"]
  properties = type["properties"]
  if properties
    # struct {
    universal_transport_types_header.push_c_line_with(string_c_struct_s_with_type_name(name)," {")
    universal_transport_types_header.push_c_line_with("int isa;")
    properties.each do |property_name, property_type|
      universal_transport_types_header.push_c_line_with(string_c_type_for_property_type(property_type)," ",property_name,";")
    end
    universal_transport_types_header.push_c_line_with("};")
    # };
  end
  
  # DEFINE TYPE SIZE
  universal_transport_types_header.push_c_line_with("#define UNIVERSAL_TRANSPORT_TYPE_", name.upcase, "_SIZE ", properties.size.to_s)
  # typedef struct
  universal_transport_types_header.push_c_line_with("typedef ", string_c_struct_s_with_type_name(name), " * ", string_c_struct_t_with_type_name(name), ";")
  # copy function
  universal_transport_types_header.push_c_line_with(string_c_struct_t_with_type_name(name)," transport_type_",name,"_copy(",string_c_struct_t_with_type_name(name)," object",");")
  # create function
  universal_transport_types_header.push_c_line_with(string_c_struct_t_with_type_name(name)," transport_type_",name,"_create(",generate_c_function_arguments_for_properties(properties),");")
  # release function
  universal_transport_types_header.push_c_line_with("void transport_type_",name,"_release(",string_c_struct_t_with_type_name(name)," object",");")
  # log function
  universal_transport_types_header.push_c_line_with("void transport_type_",name,"_log("+string_c_struct_t_with_type_name(name)," object",");")
end

# Generate the 'universal_transport_types.h' header file
generate_c_file_with universal_transport_types_header, 'universal_transport_types_prefix.h', 'universal_transport_types_suffix.h', 'universal_transport_types.h'

#############################
# universal_transport_types.c
#############################

universal_transport_types_source = []

types.each do |type| 
  name = type["name"]
  properties = type["properties"]
  
  # copy function
  universal_transport_types_source.push_c_line_with(string_c_struct_t_with_type_name(name)," transport_type_",name,"_copy(",string_c_struct_t_with_type_name(name)," ","object","){")
  universal_transport_types_source.push_c_line_with("return ","transport_type_",name,"_create(",generate_c_function_parameters_for_properties_with_prefix(properties, "object->"),");")
  universal_transport_types_source.push_c_line_with("}")
  
  # create function
  universal_transport_types_source.push_c_line_with(string_c_struct_t_with_type_name(name)," transport_type_",name,"_create(",generate_c_function_arguments_for_properties(properties),"){")
  universal_transport_types_source.push_c_line_with(string_c_struct_t_with_type_name(name), " object = (",string_c_struct_t_with_type_name(name),")calloc(1, sizeof(",string_c_struct_s_with_type_name(name),")",");")
  universal_transport_types_source.push_c_line_with("if(object){")
  universal_transport_types_source.push_c_line_with("object->isa = ","UNIVERSAL_TRANSPORT_TYPE_",name.upcase,";")
  properties.each do |property_name, property_type|
    universal_transport_types_source.push_c_line_with(string_c_property_assign_create("object", property_name, property_type))
  end
  universal_transport_types_source.push_c_line_with("}")
  universal_transport_types_source.push_c_line_with("return object;")
  universal_transport_types_source.push_c_line_with("}")
  
  # release function
  universal_transport_types_source.push_c_line_with("void transport_type_",name,"_release(",string_c_struct_t_with_type_name(name)," ","object","){")
  universal_transport_types_source.push_c_line_with("if(object){")
  properties.each do |property_name, property_type|
    universal_transport_types_source.push_c_line_with(string_c_property_release("object", property_name, property_type))
  end
  universal_transport_types_source.push_c_line_with("free(object);")
  universal_transport_types_source.push_c_line_with("}")
  universal_transport_types_source.push_c_line_with("}")
  
  # log function
  universal_transport_types_source.push_c_line_with("void transport_type_",name,"_log(",string_c_struct_t_with_type_name(name)," ","object","){")
  log_c_line = "Logc(\"{\");"
  properties.each do |property_name, property_type|
    log_c_line += string_c_property_log("object", property_name, property_type)
  end
  log_c_line += "Logc(\"}\");"
  universal_transport_types_source.push_c_line_with(log_c_line)
  universal_transport_types_source.push_c_line_with("}")
end

# Generate the 'universal_transport_types.c' source file
generate_c_file_with universal_transport_types_source, 'universal_transport_types_prefix.c', 'universal_transport_types_suffix.c', 'universal_transport_types.c'


