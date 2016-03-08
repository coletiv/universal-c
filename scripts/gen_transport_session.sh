scriptdir="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $scriptdir

ragel -C -G2 ../lib/universal_transport_session.rl -o ../lib/universal_transport_session.c