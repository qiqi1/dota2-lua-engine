#pragma once

#include "..\..\SignaturePatternResolver\SignaturePatternResolver\signature_pattern_retriever.h"
#include "utils.h"

class GlobalAddressRetriever {
public:
  static GlobalAddressRetriever& GetInstance();

  uint32_t GetStaticAddress(std::string name) {
    return static_addresses_.GetAddress(name);
  }

  uint32_t GetDynamicAddress(std::string name) {
    return dynamic_addresses_.GetDynamicSignature(name);
  }

private:
  GlobalAddressRetriever() : static_addresses_("static"), dynamic_addresses_("dynamic") {

    try {
      static_addresses_.LoadSignatures(utils::GetModuleDirectory() + "config.yaml");
    } catch( boost::exception &e) {
      utils::Log("hook: GlobalAddressRetrieverError");
    } catch (...) {
      utils::Log("hook: GlobalAddressRetrieverError");
    }

    for (auto address : static_addresses_) {
      utils::Log("hook: static: %s 0x%x", address.first.c_str(), address.second);
    }

    try {
      dynamic_addresses_.LoadSignatures(utils::GetModuleDirectory() + "config.yaml");
    } catch( boost::exception &e) {
      utils::Log("hook: GlobalAddressRetrieverError");
    } catch (...) {
      utils::Log("hook: GlobalAddressRetrieverError");
    }

    for (auto address : dynamic_addresses_) {
      utils::Log("hook: dynamic: %s 0x%x", address.first.c_str(), address.second);
    }
  }

  addressretriever::InternalAddressRetriever static_addresses_;
  addressretriever::DynamicInternalAddressRetriever dynamic_addresses_;
};