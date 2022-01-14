/**
 *  C program for a Deterministic Finite Machine
 *  https://en.wikipedia.org/wiki/Deterministic_finite_automaton
 *
 *  A DFM uses a 5-tuple for its operations
 *  The tuple contains:
 *      - States
 *      - Alphabet
 *      - Transition function
 *      - Start State
 *      - Accept State(s)
 *
 */
#include <stdio.h>
#include <stdlib.h>

// Valid DFM states
typedef enum DFM_States {
	DFM_STATE_1 = 0,
	DFM_STATE_2,
	DFM_STATE_3,
	DFM_STATE_4
} DFM_States;

// Valid DFM alphabet 
typedef enum DFM_Alphabet {
	DFM_ZERO = 0,
	DFM_ONE
} DFM_Alphabet;

// Call back for the transition function 
typedef DFM_States (*DFM_func_cb)(DFM_States state, DFM_Alphabet op);

// DFM data structure for the 5-tuple
typedef struct DFM_Machine {
	DFM_States*    states;
	DFM_Alphabet*  alphabet;
	DFM_func_cb    trans_func_cb;
	DFM_States     start_state;
	DFM_States*    accept_states;
} DFM_Machine;

// Transition function for the DFM
// Given a state and opcode from the alphabet, output the next state
DFM_States trans_func(DFM_States state, DFM_Alphabet op) {
	DFM_States next_state;
	switch (state) {
		case DFM_STATE_1:
			next_state = op == DFM_ONE ? DFM_STATE_3 : DFM_STATE_1;
			break;
		case DFM_STATE_2:
			next_state = op == DFM_ZERO ? DFM_STATE_1 : DFM_STATE_4;
			break;
		case DFM_STATE_3:
			next_state = op == DFM_ZERO ? DFM_STATE_4 : DFM_STATE_3;
			break;
		case DFM_STATE_4:
			next_state = op == DFM_ZERO ? DFM_STATE_2 : DFM_STATE_3;
			break;
		default: break;
	}
	return next_state;
}

// Create and return a new DFM
DFM_Machine* make_dfm(DFM_States start, DFM_States* end_states, DFM_func_cb func) {
	DFM_Machine* machine = (DFM_Machine*)malloc(sizeof(DFM_Machine));
	machine->states = (DFM_States*)malloc(sizeof(DFM_States) * 4);
	machine->alphabet = (DFM_Alphabet*)malloc(sizeof(DFM_Alphabet) * 2);
	if (!machine || !machine->states || !machine->alphabet) exit(1);
	machine->start_state = start;
	machine->accept_states = end_states;
	machine->trans_func_cb = func;
	for (int i = 0; i < 4; ++i) {
		machine->states[i] = i;
		if (i < 2) machine->alphabet[i] = i;
	}
	return machine;
}

// Run the DFM and see if the input string is accepted or rejected
void run_dfm(DFM_Machine* dfm, DFM_Alphabet* input_str, int n) {
	DFM_States start = dfm->start_state;
	DFM_States next_state = dfm->trans_func_cb(start, input_str[0]);
	for (int i = 1; i < n; ++i) {
		next_state = dfm->trans_func_cb(next_state, input_str[i]);
	}
	int accept_check = next_state == DFM_STATE_1 || next_state == DFM_STATE_3;
	printf("The input string: ");
	for (int i = 0; i < n; ++i) {
		printf("%d", input_str[i]);
	}
	printf(accept_check ? " is valid!\n" : " is rejected!\n");
}

// Run the DFM with info from te cmd line
void test_dfm_cmd_line(DFM_Machine* dfm, char* str_op) {
	DFM_Alphabet input_string[100];
	int i;
	for (i = 0; i < 100 && str_op[i] != '\0'; ++i) {
		if (str_op[i] != '0' && str_op[i] != '1') {
			printf("Invalid character in input string\n");
			printf("%c is not a part of the machine alphabet\n", str_op[i]);
			return;
		}
		input_string[i] = str_op[i] == '1' ? DFM_ONE : DFM_ZERO;
	}
	run_dfm(dfm, input_string, i);
}

// Entry point
int main(int argc, char* argv[]) {
	DFM_States   end_states[] = { DFM_STATE_1, DFM_STATE_3 };
	DFM_Machine* machine = make_dfm(DFM_STATE_2, end_states, trans_func);
	DFM_Alphabet input_string[] = { DFM_ONE, DFM_ONE, DFM_ZERO, DFM_ONE };
	DFM_Alphabet input_string_two[] = { DFM_ONE, DFM_ONE, DFM_ZERO, DFM_ZERO };
	if (argc > 1) {
		test_dfm_cmd_line(machine, argv[1]);
	}
	else {
		run_dfm(machine, input_string, 4);
		run_dfm(machine, input_string_two, 4);
	}
	free(machine->states);
	free(machine->alphabet);
	free(machine);
	return 0;
}
 
