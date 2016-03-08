load 'helper_functions.rb'

types = load_types(ARGV[0])

#############################
# universal_transport_messages.h
#############################

universal_transport_messages_header = []

# messages enum
universal_transport_messages_header.push_c_line_with("enum {")
types.each_with_index do |type, type_index| 
  name = type["name"]
  messages = type["messages"]
  if messages
    messages.each_with_index do |message, message_index|
      universal_transport_messages_header.push_c_line_with("UNIVERSAL_TRANSPORT_MESSAGE_",name.upcase,"_",message.upcase," = ",(type_index+1).to_s,(message_index+1).to_s,",")
    end
  end
end
universal_transport_messages_header.push_c_line_with("};")

# Generate the 'universal_transport_messages.h' header file
generate_c_file_with universal_transport_messages_header, 'universal_transport_messages_prefix.h', 'universal_transport_messages_suffix.h', 'universal_transport_messages.h'

#############################
# universal_transport_messages_internal.h
#############################

universal_transport_messages_header_internal = []

types.each_with_index do |type, type_index| 
  name = type["name"]
  universal_transport_messages_header_internal.push_c_line_with("void transport_message_pack_",name,"(msgpack_packer * msgpack_pk,",string_c_struct_t_with_type_name(name)," object);")
  universal_transport_messages_header_internal.push_c_line_with("void transport_message_unpack_",name,"(msgpack_object * msgpack_obj,",string_c_struct_t_with_type_name(name)," * object);")
end

# Generate the 'universal_transport_messages.h' header file
generate_c_file_with universal_transport_messages_header_internal, 'universal_transport_messages_internal_prefix.h', 'universal_transport_messages_internal_suffix.h', 'universal_transport_messages_internal.h'

#############################
# universal_transport_messages.c
#############################

universal_transport_messages_source = []

# message pack
universal_transport_messages_source.push_c_line_with("void transport_message_pack(transport_message_t transport_message, uint8_t name, transport_type_t arg){")
universal_transport_messages_source.push_c_line_with("msgpack_packer * pk = msgpack_packer_new(transport_message, msgpack_sbuffer_write);")
universal_transport_messages_source.push_c_line_with("switch(name){")
types.each_with_index do |type, type_index| 
  name = type["name"]
  messages = type["messages"]
  if messages
    messages.each_with_index do |message, message_index|
      universal_transport_messages_source.push_c_line_with("case UNIVERSAL_TRANSPORT_MESSAGE_",name.upcase,"_",message.upcase,":")
      universal_transport_messages_source.push_c_line_with("{")
      universal_transport_messages_source.push_c_line_with("msgpack_pack_array(pk, UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE);")
      universal_transport_messages_source.push_c_line_with("const char * type = \"",name,"\";")
      universal_transport_messages_source.push_c_line_with("msgpack_pack_str(pk, strlen(type));")
      universal_transport_messages_source.push_c_line_with("msgpack_pack_str_body(pk, type, strlen(type));")
      universal_transport_messages_source.push_c_line_with("const char * message = \"",message, "\";")
      universal_transport_messages_source.push_c_line_with("msgpack_pack_str(pk, strlen(message));")
      universal_transport_messages_source.push_c_line_with("msgpack_pack_str_body(pk, message, strlen(message));")
      universal_transport_messages_source.push_c_line_with("transport_message_pack_",name,"(pk, (",string_c_struct_t_with_type_name(name),")arg);")
      universal_transport_messages_source.push_c_line_with("} break;")
    end
  end
end
universal_transport_messages_source.push_c_line_with("}")
universal_transport_messages_source.push_c_line_with("msgpack_packer_free(pk);")
universal_transport_messages_source.push_c_line_with("}")

# message unpack
universal_transport_messages_source.push_c_line_with("int transport_message_unpack(transport_message_t transport_message, uint8_t * name, transport_type_t * arg){")
universal_transport_messages_source.push_c_line_with("int no_err = 0;")
universal_transport_messages_source.push_c_line_with("msgpack_unpacked msg;")
universal_transport_messages_source.push_c_line_with("msgpack_unpacked_init(&msg);")
universal_transport_messages_source.push_c_line_with("if(msgpack_unpack_next(&msg, transport_message->data, transport_message->size, NULL)){")
universal_transport_messages_source.push_c_line_with("msgpack_object obj = msg.data;")
universal_transport_messages_source.push_c_line_with("msgpack_object_print(stdout, obj);")
universal_transport_messages_source.push_c_line_with("if(obj.type == MSGPACK_OBJECT_ARRAY){")
universal_transport_messages_source.push_c_line_with("if(obj.via.array.size == UNIVERSAL_TRANSPORT_MESSAGE_ARRAY_SIZE){")
universal_transport_messages_source.push_c_line_with("msgpack_object * array_obj = obj.via.array.ptr;")
universal_transport_messages_source.push_c_line_with("if(array_obj->type == MSGPACK_OBJECT_STR){")
universal_transport_messages_source.push_c_line_with("char * type = (char *)calloc(array_obj->via.str.size+1, sizeof(char));")
universal_transport_messages_source.push_c_line_with("strncpy(type, array_obj->via.str.ptr, array_obj->via.str.size);")
types.each_with_index do |type, type_index| 
  name = type["name"]
  if_type_statement = type_index == 0 ? "if" : "else if"
  universal_transport_messages_source.push_c_line_with(if_type_statement,"(strcmp(type,\"",name,"\") == 0){")
  universal_transport_messages_source.push_c_line_with("++array_obj;")
  universal_transport_messages_source.push_c_line_with("if(array_obj->type == MSGPACK_OBJECT_STR){")
  universal_transport_messages_source.push_c_line_with("char * action = (char *)calloc(array_obj->via.str.size+1, sizeof(char));")
  universal_transport_messages_source.push_c_line_with("strncpy(action, array_obj->via.str.ptr, array_obj->via.str.size);")
  messages = type["messages"]
  if messages
    messages.each_with_index do |message, message_index|
      if_message_statement = message_index == 0 ? "if" : "else if"
      universal_transport_messages_source.push_c_line_with(if_message_statement," (strcmp(action, \"",message,"\") == 0){")
      universal_transport_messages_source.push_c_line_with("*name=", "UNIVERSAL_TRANSPORT_MESSAGE_",name.upcase,"_",message.upcase,";")
      universal_transport_messages_source.push_c_line_with("}")
    end
    universal_transport_messages_source.push_c_line_with("++array_obj;")
    universal_transport_messages_source.push_c_line_with("if(array_obj->type == MSGPACK_OBJECT_MAP){")
    universal_transport_messages_source.push_c_line_with("transport_message_unpack_",name,"(array_obj,(",string_c_struct_t_with_type_name(name),"*)arg);")
    universal_transport_messages_source.push_c_line_with("}")
    universal_transport_messages_source.push_c_line_with("free(action);")
  end
  universal_transport_messages_source.push_c_line_with("}")
  universal_transport_messages_source.push_c_line_with("}")
end
universal_transport_messages_source.push_c_line_with("free(type);")
universal_transport_messages_source.push_c_line_with("}")
universal_transport_messages_source.push_c_line_with("}")
universal_transport_messages_source.push_c_line_with("}")
universal_transport_messages_source.push_c_line_with("}")
universal_transport_messages_source.push_c_line_with("msgpack_unpacked_destroy(&msg);")
universal_transport_messages_source.push_c_line_with("return no_err;")
universal_transport_messages_source.push_c_line_with("}")

# pack/unpack types
types.each_with_index do |type, type_index|
  name = type["name"]
  properties = type["properties"]
  
  # pack
  universal_transport_messages_source.push_c_line_with("void transport_message_pack_",name,"(msgpack_packer * msgpack_pk,",string_c_struct_t_with_type_name(name), " object){")
  universal_transport_messages_source.push_c_line_with("msgpack_pack_map(","msgpack_pk,","UNIVERSAL_TRANSPORT_TYPE_",name.upcase,"_SIZE",");")
  properties.each do |property_name, property_type|
      universal_transport_messages_source.push_c_line_with(string_c_property_pack("msgpack_pk", "object->"+property_name, property_name, property_type))
  end
  universal_transport_messages_source.push_c_line_with("}")
  
  # unpack
  universal_transport_messages_source.push_c_line_with("void transport_message_unpack_",name,"(msgpack_object * msgpack_obj,",string_c_struct_t_with_type_name(name), "* object){")
  universal_transport_messages_source.push_c_line_with("*object = NULL;")
  universal_transport_messages_source.push_c_line_with("if (msgpack_obj->type == MSGPACK_OBJECT_MAP) {")
  universal_transport_messages_source.push_c_line_with("if (msgpack_obj->via.map.size == ","UNIVERSAL_TRANSPORT_TYPE_",name.upcase,"_SIZE",") {")
  properties.each do |property_name, property_type|
    universal_transport_messages_source.push_c_line_with(string_c_type_for_property_type(property_type),property_name,"=NULL;")
  end
  universal_transport_messages_source.push_c_line_with("msgpack_object_kv* const kv_end = msgpack_obj->via.map.ptr + msgpack_obj->via.map.size;")
  universal_transport_messages_source.push_c_line_with("for(msgpack_object_kv* kv = msgpack_obj->via.map.ptr; kv < kv_end; kv++){")
  universal_transport_messages_source.push_c_line_with("msgpack_object k_obj = kv->key;")
  universal_transport_messages_source.push_c_line_with("char * key = (char * )calloc(k_obj.via.str.size+1, sizeof(char));")
  universal_transport_messages_source.push_c_line_with("strncpy(key, k_obj.via.str.ptr, k_obj.via.str.size);")
  universal_transport_messages_source.push_c_line_with("msgpack_object v_obj = kv->val;")
  properties.each do |property_name, property_type|
    universal_transport_messages_source.push_c_line_with(string_c_property_unpack("v_obj", "key", property_name, property_name, property_type))
  end
  universal_transport_messages_source.push_c_line_with("free((void*)key);") 
  universal_transport_messages_source.push_c_line_with("}")
  universal_transport_messages_source.push_c_line_with("*object = transport_type_",name,"_create(",generate_c_function_parameters_for_properties_with_prefix(properties,""),");")
  properties.each do |property_name, property_type|
    universal_transport_messages_source.push_c_line_with("if(",property_name,"!=NULL){")
    universal_transport_messages_source.push_c_line_with("free((void*)",property_name,");")
    universal_transport_messages_source.push_c_line_with("}")
  end
  universal_transport_messages_source.push_c_line_with("}")
  universal_transport_messages_source.push_c_line_with("}")
  universal_transport_messages_source.push_c_line_with("}")
end

# Generate the 'universal_transport_messages.c' source file
generate_c_file_with universal_transport_messages_source, 'universal_transport_messages_prefix.c', 'universal_transport_messages_suffix.c', 'universal_transport_messages.c'

