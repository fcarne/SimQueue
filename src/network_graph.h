/*******************************************************
 NETWORK TOPOLOGY GRAPH
 *******************************************************/
#ifndef NETWORK_GRAPH_H_
#define NETWORK_GRAPH_H_

#include <vector>
#include <unordered_map>
#include <string>
#include "buffer.h"

class network_graph {
	std::unordered_map<std::string, network_buffer*> buffers_map;
	std::vector<network_buffer*> buffers;
	std::vector<network_buffer*> entry_buffers;
	std::vector<network_buffer*> exit_connected_buffers;

	bool check();
public:
	double tot_packets;
	double tot_transfer;
	network_graph();
	~network_graph();

	static network_graph* parse(std::string);
	void add_buffer(std::string id, network_buffer *b);
	void add_buffer(std::string id, long capacity);
	void add_connection(std::string from, edge e);
	void add_connection(std::string from, std::string to, double p);
	void set_entry(std::string id);
	void set_exit_connected(std::string id);
	const std::vector<network_buffer*> get_buffers();
	const std::vector<network_buffer*> get_entry_buffers();
	const std::vector<network_buffer*> get_exit_connected_buffers();

	void serialize(const char*, const char*);

	void to_string() {
		for (auto b : buffers) {
			printf("%p: %ld -> { ", b, b->get_capacity());
			for (auto &e : b->get_connected()) {
				if (e.buf == network_buffer::OUT) {
					printf("(EXIT, %lf), ", e.prob);
				} else {
					printf("(%p, %lf), ", e.buf, e.prob);
				}
			}
			printf("}\n");
		}
	}
};

#endif /* NETWORK_GRAPH_H_ */
