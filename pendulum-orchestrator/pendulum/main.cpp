#include "main.h"

grpc::Status pendulum_service::step(
    grpc::ServerContext *,
    const pendulum::request_step *request,
    pendulum::state *reply) {
	model_->step(request->tau());
	reply->set_time(model_->get_time());
	reply->set_theta(model_->get_theta());
	reply->set_omega(model_->get_omega());
	return grpc::Status::OK;
}

grpc::Status pendulum_service::reset(
    grpc::ServerContext *,
    const pendulum::request_reset *request,
    pendulum::state *reply) {
	model_->reset(request->theta(), request->omega());
	reply->set_time(model_->get_time());
	reply->set_theta(model_->get_theta());
	reply->set_omega(model_->get_omega());
	return grpc::Status::OK;
}

grpc::Status pendulum_service::stream_state(
    grpc::ServerContext *,
    const pendulum::empty *,
    grpc::ServerWriter<pendulum::state> *writer) {
	while (true) {
		pendulum::state state;
		state.set_time(model_->get_time());
		state.set_theta(model_->get_theta());
		state.set_omega(model_->get_omega());
		if (writer->Write(state) == false) {
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return grpc::Status::OK;
}

pendulum_service::pendulum_service() {
	model_ = std::make_unique<pendulum_simple>();
}

int main(int, char **) {
	pendulum_service service;
	grpc::ServerBuilder builder;
	builder.AddListeningPort(
	    "0.0.0.0:50051",
	    grpc::InsecureServerCredentials());

	builder.RegisterService(&service);
	std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
	server->Wait();
	return 0;
}