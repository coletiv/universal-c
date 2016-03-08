scriptdir="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $scriptdir/gen_transport_types_messages

ruby ./generate_types.rb ../../lib/universal_transport_types.yml
ruby ./generate_messages.rb ../../lib/universal_transport_types.yml

mv universal_transport_types.h ../../lib/universal_transport_types.h
mv universal_transport_types.c ../../lib/universal_transport_types.c
mv universal_transport_messages.h ../../lib/universal_transport_messages.h
mv universal_transport_messages_internal.h ../../lib/universal_transport_messages_internal.h
mv universal_transport_messages.c ../../lib/universal_transport_messages.c