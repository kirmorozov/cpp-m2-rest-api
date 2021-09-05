
using m2_encryptor = mg::m2::Encryptor;

using dbClient = std::shared_ptr<Client>;
using sharedEncryptor = std::shared_ptr<m2_encryptor>;

namespace App {
    class Core {
    public:
        dbClient dbConnection;
        sharedEncryptor encryptor;
    };
}