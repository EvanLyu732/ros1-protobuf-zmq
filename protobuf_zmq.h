//
// Created by ubuntu on 23-4-25.
//

#ifndef PROTOBUF_ZMQ_PROTOBUF_ZMQ_H
#define PROTOBUF_ZMQ_PROTOBUF_ZMQ_H

#ifndef __GNUC__
#error "Currrent Lib, Only support GNU GCC Compiler."
#endif

#ifndef __ZMQ_HPP_INCLUDED__
#include <zmq.hpp>
#endif
/*
 *  @description:
 *          This is a Header only library, Factory Method to produce Msg
 * Publisher and Subscriber. Only support ZMQ IPC  in current version. Map
 * channel file: /tmp/ + hash(protobuf_msg + pid) + ipc extension
 */
#include <cstdio>
#include <future>
#include <memory>
#include <string>
#include <unordered_map>

namespace zmq_protobuf {
inline namespace {
/*
 * @description:
 *      This class is for driver encapsulated.
 */
template <typename Msg>
struct ZmqIPCPublisher
    : public std::enable_shared_from_this<ZmqIPCPublisher<Msg>> {
  ZmqIPCPublisher(const char *channel_file) {
    context_ = ::zmq::context_t(1);
    socket_ = new ::zmq::socket_t(context_, ZMQ_PUB);
    std::string channel_name = "ipc:///tmp/" + std::string(channel_file);
    socket_->bind(channel_name);
  }

  ~ZmqIPCPublisher() {
    if (socket_)
      delete socket_;
  }

  void sync_send(Msg m) noexcept {
    std::string content_str = m.DebugString();
    ::zmq::message_t request(content_str.length());
    ::std::memcpy(request.data(), content_str.c_str(), content_str.length());

    // use same prefix, cuz we use different channel file
    zmq::message_t prefix('z');
    this->socket_->send(prefix, ::zmq::send_flags::sndmore);
    this->socket_->send(request);
  }

private:
  ::zmq::socket_t *socket_;
  ::zmq::context_t context_;
};

/*
 * @description:
 *    This class is for msg subscriber.
 */
template <typename Msg>
struct ZmqIPCSubscriber
    : public std::enable_shared_from_this<ZmqIPCSubscriber<Msg>> {
  typedef void (*MsgCallBack)(const Msg m);
  ZmqIPCSubscriber(const char *channel_file) {
    context_ = zmq::context_t(1);
    socket_ = new ::zmq::socket_t(context_, ZMQ_SUB);
    std::string endpoint = "ipc:///tmp/" + std::string(channel_file);
    socket_->connect(endpoint);
    //    socket_->setsockopt(ZMQ_SUBSCRIBE, "z");
    socket_->setsockopt(ZMQ_SUBSCRIBE, "", 0);
  }

  ~ZmqIPCSubscriber() {
    if (socket_)
      delete socket_;
  }

  inline void bindCallBack(MsgCallBack cb) noexcept { cb_ = cb; }

  [[noreturn]] void sync_run() noexcept {
    while (true) {
      ::zmq::message_t raw_msg;
      socket_->recv(&raw_msg);
      std::string msg =
          std::string(static_cast<char *>(raw_msg.data()), raw_msg.size());
      if (msg == "z")
        continue;
      Msg m;
      if (m.ParseFromString(msg)) {
        cb_(m);
      } else {
        puts("error parse msg :(, This should't happend. ");
      }
    }
  }

private:
  MsgCallBack cb_;
  ::zmq::socket_t *socket_;
  ::zmq::context_t context_;
};

template <typename Msg> struct ZmqFactory {
  static std::shared_ptr<ZmqIPCSubscriber<Msg>>
  produceIPCSubscriber(const char *channel_file) noexcept {
    return std::make_shared<ZmqIPCSubscriber<Msg>>(channel_file);
  }

  static std::shared_ptr<ZmqIPCPublisher<Msg>>
  produceIPCPublisher(const char *channel_file) noexcept {
    return std::make_shared<ZmqIPCPublisher<Msg>>(channel_file);
  }

  ZmqFactory() = default;
  // load config file to map

  ~ZmqFactory() = default;
  // dump json. map file to /tmp/zmq_channel_config.json

  void show_config() noexcept {
    // dump config file
  }

private:
  static std::unordered_map<std::string /* device */,
                            std::string /* channel_file */>
      record_map;
  static constexpr const char *device_record_ =
      "/opt/proto_zmq/device_record.json";
};

/*
 * @: trigger function
 */

} // namespace

} // namespace zmq_protobuf

#endif // PROTOBUF_ZMQ_PROTOBUF_ZMQ_H
