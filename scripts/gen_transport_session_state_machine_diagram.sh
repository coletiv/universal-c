scriptdir="$( cd -P "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
cd $scriptdir

ragel -V ../lib/universal_transport_session.rl > ../docs/transport_session_state_machine.dot
dot -Tpng ../docs/transport_session_state_machine.dot > ../docs/transport_session_state_machine.png
rm ../docs/transport_session_state_machine.dot