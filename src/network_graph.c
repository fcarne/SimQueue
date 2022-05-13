/* -*- C++ -*- */
/*******************************************************
 NETWORK TOPOLOGY GRAPH
 *******************************************************/

#include "network_graph.h"
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <cerrno>

network_graph::network_graph() {
	tot_packets = 0.0;
	tot_transfer = 0.0;
}

network_graph::~network_graph() {
	for (auto b : buffers)
		delete b;
}

bool network_graph::check() {
	for (auto const &b : buffers_map) {
		if (!b.second->is_valid()) {
			fprintf(stderr, "Buffer %s probabilities do not add up to 1",
					b.first.c_str());
			return false;
		}
	}

	if (entry_buffers.size() == 0)
		fprintf(stderr, "There are no entry buffers");
	if (exit_connected_buffers.size() == 0)
		fprintf(stderr, "There are no buffers connected to the outside");

	return entry_buffers.size() > 0 && exit_connected_buffers.size() > 0;
}

network_graph* network_graph::parse(std::string filename) {
	network_graph *network = new network_graph();

	if (FILE *file = fopen(filename.c_str(), "r")) {
		char buf[25];

		while (EOF != fscanf(file, "%25[^\n]\n", buf)) {
			if (buf[0] == '#') {
				char id[10];
				long capacity = 0;

				sscanf(buf, "#%[^:]: %ld", id, &capacity);
				network->add_buffer(id, capacity);
			} else if (strstr(buf, "->")) {
				char from[10];
				char to[10];
				double p = 0;

				sscanf(buf, "%[^->]->%[^:]: %lf", from, to, &p);
				if (strcmp(to, "EXIT") == 0) {
					network->set_exit_connected(from);
					network->add_connection(from, { network_buffer::EXIT, p });
				} else
					network->add_connection(from, to, p);
			} else if (strstr(buf, "ENTRY: ")) {
				char id[10];
				sscanf(buf, "ENTRY: %s", id);
				network->set_entry(id);
			} else {
				fprintf(stdout, "skip line\n");
			}
		}

		fclose(file);
	} else {
		fprintf(stderr, "File doesn't exist or can't be read!");
		exit(-1);
	}

	if (!network->check()) {
		fprintf(stderr, " - Parsed network is invalid!\n");
		exit(-1);
	}

	return network;
}

void network_graph::add_buffer(std::string id, network_buffer *b) {
	buffers.push_back(b);
	buffers_map[id] = b;
}

void network_graph::add_buffer(std::string id, long capacity) {
	add_buffer(id, new network_buffer(capacity));
}

void network_graph::add_connection(std::string from, edge e) {
	if (buffers_map.find(from) != buffers_map.end())
		buffers_map[from]->add_next(e);
}

void network_graph::add_connection(std::string from, std::string to, double p) {
	if (buffers_map.find(to) != buffers_map.end())
		add_connection(from, { buffers_map[to], p });
}

void network_graph::set_entry(std::string id) {
	if (buffers_map.find(id) != buffers_map.end())
		entry_buffers.push_back(buffers_map[id]);
}

void network_graph::set_exit_connected(std::string id) {
	if (buffers_map.find(id) != buffers_map.end())
		exit_connected_buffers.push_back(buffers_map[id]);
}

const std::vector<network_buffer*> network_graph::get_buffers() {
	return buffers;
}
const std::vector<network_buffer*> network_graph::get_entry_buffers() {
	return entry_buffers;
}
const std::vector<network_buffer*> network_graph::get_exit_connected_buffers() {
	return exit_connected_buffers;
}

void network_graph::serialize(const char *prob_filename,
		const char *node_filename) {

	extern double inter;
	extern long L;

	std::unordered_map<network_buffer*, int> buffers_idx;
	for (size_t i = 0; i < buffers.size(); ++i) {
		buffers_idx[buffers[i]] = i + 1;
	}

	FILE *f_prob = fopen64(prob_filename, "w");
	FILE *f_node = fopen64(node_filename, "w");

	for (auto b : buffers) {
		int idx = buffers_idx[b];
		fprintf(f_prob, "%d %d %d\n", idx, idx, 1);
		for (auto n : b->get_connected()) {
			if (n.buf == network_buffer::EXIT)
				continue;

			fprintf(f_prob, "%d %d %lf\n", buffers_idx[(network_buffer*) n.buf],
					idx, -n.prob);
		}
		fprintf(f_node, "%lf %lf\n",
				std::find(entry_buffers.begin(), entry_buffers.end(), b)
						!= entry_buffers.end() ? 1 / inter : 0,
				b->get_capacity() / (double) L);
	}

	fclose(f_prob);
	fclose(f_node);
}
