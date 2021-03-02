#ifndef SRC_PROTOCOL_MESSAGE_H_
#define SRC_PROTOCOL_MESSAGE_H_

#include <memory>
#include <string>

enum class MessageType {
  MESSAGE_OFFER,
  MESSAGE_ANSWER,
  MESSAGE_CANDIDATE,
  MESSAGE_LOGIN,
  MESSAGE_LOGIN_RESPONSE,
};

static const char* kMessageTypeStr[] = {"offer", "answer", "candidate", "login",
                                        "login_response"};

class Message {
 public:
  Message(const MessageType type) : type_(type) {}
  virtual ~Message() {}

  virtual std::string SerializeAsJsonString() = 0;

  static std::shared_ptr<Message> ParseMessage(const std::string& json_str);

  MessageType type() const { return type_; }

  std::string type_string() const { return kMessageTypeStr[(int)type_]; }

 protected:
  MessageType type_;
};

class OfferMessage : public Message {
 public:
  OfferMessage() : Message(MessageType::MESSAGE_OFFER) {}
  ~OfferMessage() override{};

  std::string SerializeAsJsonString() override;

 private:
};

class AnswerMessage : public Message {
 public:
  AnswerMessage() : Message(MessageType::MESSAGE_ANSWER) {}
  ~AnswerMessage() override {}

  std::string SerializeAsJsonString() override;
};

class CandidateMessage : public Message {
 public:
  CandidateMessage() : Message(MessageType::MESSAGE_CANDIDATE) {}
  ~CandidateMessage() override {}

  std::string SerializeAsJsonString() override;
};

// Register message
// eg : { "type":3, "device_name" : "test123" }
class LoginMessage : public Message {
 public:
  LoginMessage() : Message(MessageType::MESSAGE_LOGIN) {}
  ~LoginMessage() override {}

  std::string SerializeAsJsonString() override;

  std::string device_name() const { return device_name_; }

  void set_device_name(const std::string& device_name) {
    device_name_ = device_name;
  }

 private:
  std::string device_name_;
};

// Register response message
// eg: { "type":4, "device_id" : "sdafafdsfdsgfd" }
class LoginResponseMessage : public Message {
 public:
  LoginResponseMessage() : Message(MessageType::MESSAGE_LOGIN_RESPONSE) {}
  ~LoginResponseMessage() override {}

  std::string SerializeAsJsonString() override;

  void set_device_id(const std::string& id) { device_id_ = id; }

 private:
  std::string device_id_;
};

#endif  // SRC_PROTOCOL_MESSAGE_H_