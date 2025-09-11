


//#ifndef CSVHANDLER_H
//#define CSVHANDLER_H

//class csvHandler {
//    public:
//        void writeCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
//            std::ofstream
//                file(filename);
//            if (!file.is_open()) {
//                std::cerr << "Error: Could not open the file!" << std::endl;
//                return;
//            }
//            for (const auto& row : data) {
//                for (size_t i = 0; i < row.size(); ++i) {
//                    file << row[i];
//                    if (i < row.size() - 1) {
//                        file << ",";
//                    }
//                }
//                file << "\n";
//            }
//            file.close();
//        }
//};
//#endif