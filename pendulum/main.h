#pragma once
#include "abstract.h"
#include "simple.h"
#include "pendulum.pb.h"
#include "pendulum.grpc.pb.h"

class pendulum_service : public pendulum::pendulum::Service {
private: /* pendulum model */
	std::unique_ptr<pendulum_abstract> model_ = nullptr;

public: /* proto */
	grpc::Status step(
	    grpc::ServerContext *context,
	    const pendulum::request_step *request,
	    pendulum::state *reply) override final;

	grpc::Status reset(
	    grpc::ServerContext *context,
	    const pendulum::request_reset *request,
	    pendulum::state *reply) override final;

	grpc::Status stream_state(
	    grpc::ServerContext *context,
	    const pendulum::empty *request,
	    grpc::ServerWriter<pendulum::state> *writer) override final;

public: /* constructor */
	pendulum_service();
};
