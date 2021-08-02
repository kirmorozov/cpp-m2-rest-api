
using m2_encryptor = mg::m2::Encryptor;

using sharedClient = std::shared_ptr<Client>;
using sharedEncryptor = std::shared_ptr<m2_encryptor>;

namespace App {
    class Core {
    public:
        sharedClient dbConnection;
        sharedEncryptor encryptor;
    };
}