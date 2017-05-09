//implementation summary of qubie blootooth communicator

#include "qubie_t.h"
#include "qubie.h"
#include "qubie_bt_communicator.h"
#include "qubie_bt_client.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

//globals
//@design for internal use
extern qubie_t the_qubie;
static bt_communicator_t *self = &the_qubie.bt_communicator;
//@design for export to bt_client which does not have direct access to the_qubie
bt_communicator_t *the_communicator = &the_qubie.bt_communicator;

//constructor
bt_communicator_t *make_bt_communicator(qubie_t *qubie){
	bt_communicator_t *bt_communicator_struct = malloc(sizeof(struct bt_communicator));
	bt_communicator_struct->subscribed = false;
	bt_communicator_struct->bt_client = NULL;
	//bt_communicator_struct->qubie = qubie;
	//@TODO bt_communicator_struct->legal_update_states=bt_communicator_legal_update_states;
	return bt_communicator_struct;
};

// ====================================================================
// @bon QUERIES
// ====================================================================

/* booleans implemented in QUBIE_INTERFACE representing specific states
 * e.g. booting, running, etc. are not needed in this abstraction and
 * are therefore left out
 */

// qubie status
/*@ ensures \result == the_state
 * 	assigns \nothing
 */
state_t bt_communicator_qubie_state(){
	return state();
};

//pointer to the qubie module that is connected to this communicator
// moved to central location: qubie_t qubie();

// pointer to qubie's log, a list of log entries with some added functionality
/*@	ensures \result == &the_qubie.log;
 * 	assigns \nothing;
 *
 */
qubie_logger_t *get_qubie_log(){
	return get_log();
};

/*@ ensures \result == self->subscribed;
 * ensures \result == bt_client_subscribed;
 * 	assigns \nothing
 */
bool subscribed(){
	return self->subscribed;
};
/*@ requires self->subscribed
 * 	ensures \result == self->bt_client;
 * 	assigns \nothing;
 */
bt_client_t *bt_client(){
	return self->bt_client;
};

/*@
 * 	ensures !self->subscribed or bt_client_qubie_state == \result;
 *	assigns \nothing
 */
bool bt_communicator_action_published( state_t the_state){
	bool Result = false;
	if (!self->subscribed) {
		Result = true; //published to the empty set or clients
	} else {
		Result = published(the_state);
	}
	return Result;
};

/*@ ensures {STOPPED, POWERED_OFF} == \result;
 */
const state_t *get_bt_communicator_legal_update_states(){
	return self->legal_update_states;
};

// ====================================================================
// @bon COMMANDS
// ====================================================================

/* specific stage changing commands implemented in QUBIE_INTERFACE
 * e.g. start_running, etc. are not needed in this abstraction and
 * are therefore left out
 */

/*@ requires !self->subscribed;
 * 	ensures self->bt_client==the_bluetooth_client;
 * 	ensures self->subscribed;
 * 	ensures bt_client_subscribed;
 * 	assigns self->subscribed, self->bt_client, bt_client_subscribed;
 */
void subscribe( bt_client_t *the_bluetooth_client){
	self->bt_client = the_bluetooth_client;
	self->subscribed = true;
};

/*@ requires self->subscribed;
 * 	ensures !self->subscribed;
 * 	ensures !bt_client_subscribed;
 * 	assigns self->subscribed, bt_client_subscribed;
 */
void unsubscribe(){
	self->bt_client = NULL;
	self->subscribed = false;
};

/*@ ensures !bt_client_subscribed || bt_client_qubie_state == the_state;
 * 	assigns the_qubie_state, bt_client_qubie_state;
 */
void bt_communicator_publish_action( state_t the_state){
	if (self->subscribed) {
		publish_from_bt_communicator(the_state);
	};
};

/*@ requires the_state == STOPPED ^^ the_state == POWERED_OFF; //design: legal states
 * 	ensures the_state == the_qubie.state;
 * 	ensures the_state == the_qubie_state;
 * 	assigns the_qubie.state, the_qubie_state;
 */
void bt_communicator_update_qubie_state( state_t the_state){
	update_state( the_state);
};

//@design allow bt_client to do whatever is in it's spec.
/*@
 * 	assigns bt_client_subscribed, bt_client_qubie_state, the_qubie_state,
 * 			self->subscribed, self->bt_client, the_qubie.state
 */
void poll_bt_communicator(){
	poll_bt_client();
};


















