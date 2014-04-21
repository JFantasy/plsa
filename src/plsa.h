#ifndef __PLSA_H
#define __PLSA_H

#include <vector>
#include <map>
#include <string>

class PLSA {
  public:
    PLSA();
    ~PLSA();

    void Initialize();
    void LoadData(const std::string &file_name);
    void Train(const int &epoch, const int &topic);
    void OutputTheta(const std::string &file_name);
    void OutputPhi(const std::string &file_name);

  private:
    void EStep();
    void MStep();

    void RandomParameters();
    void RandomTheta();
    void RandomPhi();

    void CalculatePhi();
    void CalculateTheta();

    std::map<int, std::map<int, std::map<int, double> > > mu_;
    std::map<int, std::map<int, double> > theta_, phi_;
    std::map<int, bool> words_;
    std::vector<std::map<int, int> > data_;
    int word_number_, document_number_, topic_number_;
};

#endif
