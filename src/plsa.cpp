#include "plsa.h"

#include <stdlib.h>

#include <fstream>
#include <iostream>

PLSA::PLSA() {
}

PLSA::~PLSA() {
}

void PLSA::Initialize() {
  mu_.clear();
  theta_.clear();
  phi_.clear();
  words_.clear();
  std::vector<std::map<int, int> >().swap(data_);
  word_number_ = document_number_ = 0;
}

void PLSA::LoadData(const std::string &file_name) {
  std::ifstream fin(file_name.c_str());
  int word_number_per_document;
  while (fin >> word_number_per_document) {
    std::map<int, int> document;
    for (int i = 0; i < word_number_per_document; ++ i) {
      int word, frequency;
      char quotation;
      fin >> word >> quotation >> frequency;
      document[word] += frequency;
      words_[word] = true;
    }
    data_.push_back(document);
  }
  fin.close();
  word_number_ = words_.size();
  document_number_ = data_.size();
}

void PLSA::Train(const int &epoch, const int &topic) {
  topic_number_ = topic;
  RandomParameters();
  std::cout << "Random Parameters Finished!!" << std::endl;
  for (int i = 0; i < epoch; ++ i) {
    EStep();
    MStep();
  }
}

void PLSA::EStep() {
  for (int document = 0; document < document_number_; ++ document) {
    for (auto word_iterator = words_.begin(); word_iterator != words_.end();
        ++ word_iterator) {
      double sum = 0;
      int word = word_iterator->first;
      for (int topic = 0; topic < topic_number_; ++ topic) {
        sum += phi_[topic][word] * theta_[document][topic];
      }
      for (int topic = 0; topic < topic_number_; ++ topic) {
        mu_[topic][document][word] = phi_[topic][word] * theta_[document][topic]
          / sum;
      }
    }
  }
  std::cout << "EStep Finished!!" << std::endl;
}

void PLSA::MStep() {
  CalculatePhi();
  CalculateTheta();
  std::cout << "MStep Finished!!" << std::endl;
}

void PLSA::RandomPhi() {
  const int k_random_size = 10000;
  for (int topic = 0; topic < topic_number_; ++ topic) {
    double sum = 0;
    for (auto word_iterator = words_.begin(); word_iterator != words_.end();
        ++ word_iterator) {
      int word = word_iterator->first;
      phi_[topic][word] = rand() % k_random_size * 1.0;
      sum += phi_[topic][word];
    }
    for (auto word_iterator = words_.begin(); word_iterator != words_.end();
        ++ word_iterator) {
      int word = word_iterator->first;
      phi_[topic][word] /= sum;
    }
  }
}

void PLSA::RandomTheta() {
  const int k_random_size = 10000;
  for (int document = 0; document < document_number_; ++ document) {
    double sum = 0;
    for (int topic = 0; topic < topic_number_; ++ topic) {
      theta_[document][topic] = rand() % k_random_size * 1.0;
      sum += theta_[document][topic];
    }
    for (int topic = 0; topic < topic_number_; ++ topic) {
      theta_[document][topic] /= sum;
    }
  }
}

void PLSA::RandomParameters() {
  RandomTheta();
  RandomPhi();
}

void PLSA::CalculatePhi() {
  for (int topic = 0; topic < topic_number_; ++ topic) {
    double denominator = 0;
    for (int document = 0; document < document_number_; ++ document) {
      for (auto word_iterator = words_.begin(); word_iterator != words_.end();
          ++ word_iterator) {
        int word = word_iterator->first;
        denominator += data_[document][word] * mu_[topic][document][word];
      }
    }
    for (auto word_iterator = words_.begin(); word_iterator != words_.end();
        ++ word_iterator) {
      int word = word_iterator->first;
      double numerator = 0;
      for (int document = 0; document < document_number_; ++ document) {
        numerator += data_[document][word] * mu_[topic][document][word];
      }
      phi_[topic][word] = numerator / denominator;
    }
  }
}

void PLSA::CalculateTheta() {
  for (int document = 0; document < document_number_; ++ document) {
    double denominator = 0;
    for (auto word_iterator = words_.begin(); word_iterator != words_.end();
        ++ word_iterator) {
      int word = word_iterator->first;
      denominator += data_[document][word];
    }
    for (int topic = 0; topic < topic_number_; ++ topic) {
      double numerator = 0;
      for (auto word_iterator = words_.begin(); word_iterator != words_.end();
          ++ word_iterator) {
        int word = word_iterator->first;
        numerator += data_[document][word] * mu_[topic][document][word];
      }
      theta_[document][topic] = numerator / denominator;
    }
  }
}

void PLSA::OutputTheta(const std::string &file_name) {
  std::ofstream fout(file_name.c_str());
  for (int document = 0; document < document_number_; ++ document) {
    for (int topic = 0; topic < topic_number_; ++ topic) {
      fout << theta_[document][topic] << " ";
    }
    fout << std::endl;
  }
  fout.close();
}

void PLSA::OutputPhi(const std::string &file_name) {
  std::ofstream fout(file_name.c_str());
  for (int topic = 0; topic < topic_number_; ++ topic) {
    for (auto word_iterator = words_.begin(); word_iterator != words_.end();
        ++ word_iterator) {
      int word = word_iterator->first;
      fout << phi_[topic][word] << " ";
    }
    fout << std::endl;
  }
}
