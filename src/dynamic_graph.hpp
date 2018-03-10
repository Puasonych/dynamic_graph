#include <cstdint>
#include <vector>
#include <utility>
#include <cmath>
#include <random>
#include <tuple>
#include <algorithm>

std::random_device rd;

std::mt19937 mt(rd());

namespace rbtree {
    enum class node_color
    {
        red,
        black
    };

    template <typename Key, typename Data>
    struct node
    {
        explicit node(const Key & key, const Data & data)
            : left(nullptr), right(nullptr), parent(nullptr)
        {
            info.first = key;
            info.second = data;
        }

        std::pair<Key, Data> info;
        node_color color;
        node * left;
        node * right;
        node * parent;
    };

    template <typename Key, typename Data>
    class tree
    {
    public:
        tree()
            : m_root(nullptr)
        {
        }

        ~tree()
        {
            clear();
        }

        Data & operator[](const Key & key)
        {
            node<Key, Data> * search = find(key);

            if (search)
            {
                return search->info.second;
            }
            else
            {
                const Data data{};
                return push(key, data);
            }
        }

        node<Key, Data> * find(const Key & key)
        {
            if (!m_root)
            {
                return nullptr;
            }

            return _find(m_root, key);
        }

        Data & push(const Key & key, const Data & data)
        {
            node<Key, Data> * ptr = new node<Key, Data>(key, data);

            m_root = bst_insert(m_root, ptr);

            if (ptr)
            {
                m_nodes.push_back(ptr);
                fix_error(m_root, ptr);
            }

            return ptr->info.second;
        }

        void clear()
        {
            auto step = m_nodes.size();
            while (!m_nodes.empty())
            {
                delete m_nodes[--step];
                m_nodes.pop_back();
            }

            m_root = nullptr;
        }

        uint64_t size()
        {
            return m_nodes.size();
        }

        const std::vector< node<Key, Data> * > & items = m_nodes;

    protected:
        node<Key, Data> * _find(node<Key, Data> * root, const Key & key)
        {
            while (root)
            {
                if (root->info.first == key)
                {
                    return root;
                }

                if (key < root->info.first)
                {
                    root = root->left;
                }
                else
                {
                    root = root->right;
                }
            }

            return nullptr;
        }

        void rotate_left(node<Key, Data> * & root, node<Key, Data> * & ptr)
        {
            node<Key, Data> * ptr_right = ptr->right;

            ptr->right = ptr_right->left;

            if (ptr->right)
                ptr->right->parent = ptr;

            ptr_right->parent = ptr->parent;

            if (!ptr->parent)
                root = ptr_right;

            else if (ptr == ptr->parent->left)
                ptr->parent->left = ptr_right;
            else
                ptr->parent->right = ptr_right;

            ptr_right->left = ptr;
            ptr->parent = ptr_right;
        }

        void rotate_right(node<Key, Data> * & root, node<Key, Data> * & ptr)
        {
            node<Key, Data> * ptr_left = ptr->left;

            ptr->left = ptr_left->right;

            if (ptr->left)
                ptr->left->parent = ptr;

            ptr_left->parent = ptr->parent;

            if (!ptr->parent)
                root = ptr_left;

            else if (ptr == ptr->parent->left)
                ptr->parent->left = ptr_left;

            else
                ptr->parent->right = ptr_left;

            ptr_left->right = ptr;
            ptr->parent = ptr_left;
        }

        void fix_error(node<Key, Data> * & root, node<Key, Data> * & ptr)
        {
            node<Key, Data> * parent_ptr = nullptr;
            node<Key, Data> * grand_parent_ptr = nullptr;

            while ((ptr != root) && (ptr->color != node_color::black) &&
                (ptr->parent->color == node_color::red))
            {

                parent_ptr = ptr->parent;
                grand_parent_ptr = ptr->parent->parent;

                if (parent_ptr == grand_parent_ptr->left)
                {

                    node<Key, Data> * uncle_ptr = grand_parent_ptr->right;

                    if (uncle_ptr && uncle_ptr->color == node_color::red)
                    {
                        grand_parent_ptr->color = node_color::red;
                        parent_ptr->color = node_color::black;
                        uncle_ptr->color = node_color::black;
                        ptr = grand_parent_ptr;
                    }

                    else
                    {
                        if (ptr == parent_ptr->right)
                        {
                            rotate_left(root, parent_ptr);
                            ptr = parent_ptr;
                            parent_ptr = ptr->parent;
                        }

                        rotate_right(root, grand_parent_ptr);
                        std::swap(parent_ptr->color, grand_parent_ptr->color);
                        ptr = parent_ptr;
                    }
                }
                else
                {
                    node<Key, Data>  *uncle_ptr = grand_parent_ptr->left;

                    if (uncle_ptr && uncle_ptr->color == node_color::red)
                    {
                        grand_parent_ptr->color = node_color::red;
                        parent_ptr->color = node_color::black;
                        uncle_ptr->color = node_color::black;
                        ptr = grand_parent_ptr;
                    }
                    else
                    {
                        if (ptr == parent_ptr->left)
                        {
                            rotate_right(root, parent_ptr);
                            ptr = parent_ptr;
                            parent_ptr = ptr->parent;
                        }

                        rotate_left(root, grand_parent_ptr);
                        std::swap(parent_ptr->color, grand_parent_ptr->color);
                        ptr = parent_ptr;
                    }
                }
            }

            root->color = node_color::black;
        }

        static node<Key, Data> * bst_insert(node<Key, Data> * root, node<Key, Data> * & ptr)
        {
            if (!root)
                return ptr;

            if (ptr->info.first == root->info.first)
            {
                root->info.second = ptr->info.second;

                delete ptr;
                ptr = nullptr;

                return root;
            }

            if (ptr->info.first < root->info.first)
            {
                root->left = bst_insert(root->left, ptr);
                root->left->parent = root;
            }
            else if (ptr->info.first > root->info.first)
            {
                root->right = bst_insert(root->right, ptr);
                root->right->parent = root;
            }

            return root;
        }

    private:
        node<Key, Data> * m_root;
        std::vector< node<Key, Data> * > m_nodes;
    };
}

namespace prime {

    bool is_prime(uint32_t value)
    {
        if (value == 2)
        {
            return true;
        }

        if (value % 2 == 0)
        {
            return false;
        }

        uint32_t step = 3;
        while (step * step <= value)
        {
            if (value % step == 0)
            {
                return false;
            }

            step += 2;
        }

        return true;
    }

    uint32_t prime_more_than(uint32_t value)
    {
        while (!is_prime(value))
        {
            ++value;
        }

        return value;
    }
}

namespace l0sample {

    rbtree::tree<uint32_t, uint32_t> cache_prime;

    uint32_t prime_more_than(uint32_t value)
    {
        auto * search = cache_prime.find(value);

        if (!search)
        {
            return cache_prime.push(value, prime::prime_more_than(4 * value));
        }

        return search->info.second;
    }

    int64_t fast_pow(int32_t value, uint32_t p_value, uint32_t mod)
    {
        if (p_value == 0)
        {
            return 1;
        }

        if (p_value == 1)
        {
            return value;
        }

        int32_t mult = (p_value % 2 == 1 ? value : 1);

        return mult * fast_pow(value * value % mod, p_value / 2, mod);
    }

    class hash_k
    {
    public:
        explicit hash_k(int32_t dom, uint32_t k_value = 2)
            : m_dom(dom),
            m_prime_value(prime::prime_more_than(2 * dom))
        {
            std::uniform_int_distribution<uint32_t> rand_uint32_t(0, m_prime_value - 1);

            for (auto i = 0u; i < k_value; ++i)
            {
                m_coefficients.push_back(rand_uint32_t(mt));
            }
        }

        hash_k()
            : m_dom(0), m_prime_value(0)
        {
        }

        hash_k(const hash_k & hash)
        {
            m_dom = hash.m_dom;
            m_prime_value = hash.m_prime_value;
            m_coefficients = hash.m_coefficients;
        }

        hash_k(hash_k && other) noexcept
        {
            std::swap(m_dom, other.m_dom);
            std::swap(m_prime_value, other.m_prime_value);
            std::swap(m_coefficients, other.m_coefficients);
        }

        hash_k & operator=(const hash_k & other)
        {
            m_dom = other.m_dom;
            m_coefficients = other.m_coefficients;
            m_prime_value = other.m_prime_value;

            return *this;
        }

        hash_k & operator=(hash_k && other) noexcept
        {
            std::swap(m_dom, other.m_dom);
            std::swap(m_prime_value, other.m_prime_value);
            std::swap(m_coefficients, other.m_coefficients);

            return *this;
        }

        uint32_t at(uint32_t value)
        {
            uint32_t result = 0, mult = 1;

            for (auto el : m_coefficients)
            {
                result = (result + mult * el) % m_prime_value;
                mult = (mult * value) % m_prime_value;
            }

            return result % m_dom;
        }

    private:
        int32_t m_dom;
        uint32_t m_prime_value;
        std::vector<uint32_t> m_coefficients;
    };

    class one_sparse_vector
    {
    public:
        explicit one_sparse_vector(uint32_t size, double delta)
            : m_size(size),
            m_prime_value(prime_more_than(4 * size)),
            m_s_one(0), m_s_two(0),
            m_k_value(1 - 2 * int32_t(std::ceil(std::log2(delta))))
        {
            std::uniform_int_distribution<int32_t> rand_int32_t(0, m_prime_value - 1);

            for (auto i = 0u; i < m_k_value; ++i)
            {
                m_tests.push_back(std::make_pair(0, rand_int32_t(mt)));
            }
        }

        one_sparse_vector(const one_sparse_vector & other)
        {
            m_size = other.m_size;
            m_prime_value = other.m_prime_value;
            m_k_value = other.m_k_value;
            m_s_one = other.m_s_one;
            m_s_two = other.m_s_two;
            m_tests = other.m_tests;
        }

        one_sparse_vector & operator=(const one_sparse_vector & other)
        {
            m_size = other.m_size;
            m_prime_value = other.m_prime_value;
            m_k_value = other.m_k_value;
            m_s_one = other.m_s_one;
            m_s_two = other.m_s_two;
            m_tests = other.m_tests;

            return *this;
        }

        one_sparse_vector(one_sparse_vector && other) noexcept
        {
            std::swap(m_size, other.m_size);
            std::swap(m_prime_value, other.m_prime_value);
            std::swap(m_k_value, other.m_k_value);
            std::swap(m_s_one, other.m_s_one);
            std::swap(m_s_two, other.m_s_two);
            std::swap(m_tests, other.m_tests);
        }

        one_sparse_vector & operator=(one_sparse_vector && other) noexcept
        {
            std::swap(m_size, other.m_size);
            std::swap(m_prime_value, other.m_prime_value);
            std::swap(m_k_value, other.m_k_value);
            std::swap(m_s_one, other.m_s_one);
            std::swap(m_s_two, other.m_s_two);
            std::swap(m_tests, other.m_tests);

            return *this;
        }

        void update(uint32_t index, int32_t value)
        {
            m_s_one += value;
            m_s_two += index * value;

            for (auto & test : m_tests)
            {
                test.first += value * fast_pow(test.second, index, m_prime_value) % m_prime_value;
            }
        }

        std::pair<int32_t, int32_t> recover()
        {
            return std::make_pair(m_s_two / m_s_one, m_s_one);
        }

        bool correct()
        {
            if (m_s_one == 0 || m_s_two % m_s_one != 0 || m_s_two * m_s_one < 0)
                return false;

            auto data = recover();

            for (auto & test : m_tests)
            {
                if ((data.second * fast_pow(test.second, data.first, m_prime_value)
                    - int32_t(test.first)) % m_prime_value != 0)
                {
                    return false;
                }
            }

            return true;
        }

        friend one_sparse_vector operator+(
            const one_sparse_vector & base,
            const one_sparse_vector & other)
        {
            std::vector < std::pair<int32_t, int32_t> > tests;

            for (auto i = 0; i < other.m_k_value; ++i)
            {
                tests.push_back(std::make_pair(
                    (base.m_tests[i].first + other.m_tests[i].first)
                    % other.m_prime_value,
                    other.m_tests[i].first));
            }

            return one_sparse_vector(base.m_size, base.m_prime_value,
                base.m_s_one + other.m_s_one, base.m_s_two + other.m_s_two,
                base.m_k_value, tests);
        }

    private:
        explicit one_sparse_vector(uint32_t size, uint32_t prime_value,
            int32_t s_one, int32_t s_two,
            uint32_t k_value,
            std::vector < std::pair<int32_t, int32_t> > & tests)
            : m_size(size), m_prime_value(prime_value),
            m_s_one(s_one), m_s_two(s_two), m_k_value(k_value),
            m_tests(tests)
        {
        }

    private:
        uint32_t m_size;
        uint32_t m_prime_value;

        int32_t m_s_one;
        int32_t m_s_two;
        uint32_t m_k_value;

        std::vector < std::pair<int32_t, int32_t> > m_tests;
    };

    class s_sparse_vector
    {
    public:
        explicit s_sparse_vector(uint32_t size, int32_t s_value, double delta)
            : m_cnt(0),
            m_size(size),
            m_s_value(s_value),
            m_k_value(1 - 2 * int32_t(std::ceil(std::log2(delta / 2.))))
        {
            double delta_decoder = delta / (2. * m_k_value * m_s_value);

            m_sketchs.resize(m_k_value);
            for (auto i = 0; i < m_k_value; ++i)
            {
                for (auto j = 0; j < 2 * m_s_value; ++j)
                {
                    m_sketchs[i].push_back(one_sparse_vector(m_size, delta_decoder));
                }

                m_hashes.push_back(hash_k(2 * m_s_value));
            }
        }

        s_sparse_vector(const s_sparse_vector & other)
        {
            m_size = other.m_size;
            m_s_value = other.m_s_value;
            m_cnt = other.m_cnt;
            m_k_value = other.m_k_value;
            m_sketchs = other.m_sketchs;
            m_hashes = other.m_hashes;
        }

        s_sparse_vector & operator=(const s_sparse_vector & other)
        {
            m_size = other.m_size;
            m_s_value = other.m_s_value;
            m_cnt = other.m_cnt;
            m_k_value = other.m_k_value;
            m_sketchs = other.m_sketchs;
            m_hashes = other.m_hashes;

            return *this;
        }

        s_sparse_vector(s_sparse_vector && other) noexcept
        {
            std::swap(m_size, other.m_size);
            std::swap(m_s_value, other.m_s_value);
            std::swap(m_cnt, other.m_cnt);
            std::swap(m_k_value, other.m_k_value);
            std::swap(m_sketchs, other.m_sketchs);
            std::swap(m_hashes, other.m_hashes);
        }

        s_sparse_vector & operator=(s_sparse_vector && other) noexcept
        {
            std::swap(m_size, other.m_size);
            std::swap(m_s_value, other.m_s_value);
            std::swap(m_cnt, other.m_cnt);
            std::swap(m_k_value, other.m_k_value);
            std::swap(m_sketchs, other.m_sketchs);
            std::swap(m_hashes, other.m_hashes);

            return *this;
        }

        void update(uint32_t index, int32_t value)
        {
            ++m_cnt;

            for (auto i = 0; i < m_k_value; ++i)
            {
                m_sketchs[i][m_hashes[i].at(index)].update(index, value);
            }
        }

        std::vector< std::pair<uint32_t, int32_t> > recover()
        {
            rbtree::tree<uint32_t, int32_t> tmp_r;
            std::vector< std::pair<uint32_t, int32_t> > result;

            for (auto & table : m_sketchs)
            {
                for (auto & cell : table)
                {
                    if (cell.correct())
                    {
                        auto pair = cell.recover();
                        auto * search = tmp_r.find(pair.first);
                        if (!search)
                        {
                            result.push_back(pair);
                        }

                        tmp_r.push(pair.first, pair.second);
                    }
                }
            }

            return result;
        }

        friend s_sparse_vector operator+(
            const s_sparse_vector & base,
            const s_sparse_vector & other)
        {
            std::vector< std::vector<one_sparse_vector> > sketchs;

            for (auto i = 0; i < base.m_k_value; ++i)
            {
                std::vector<one_sparse_vector> tmp_r_vector;
                for (auto j = 0; j < 2 * base.m_s_value; ++j)
                {
                    tmp_r_vector.push_back(base.m_sketchs[i][j] + other.m_sketchs[i][j]);
                }
                sketchs.push_back(tmp_r_vector);
            }

            s_sparse_vector result(base.m_size, base.m_s_value,
                base.m_cnt + other.m_cnt, base.m_k_value,
                sketchs, base.m_hashes);

            return result;
        }

        bool touched()
        {
            return m_cnt != 0;
        }

    private:
        explicit s_sparse_vector(uint32_t size, int32_t s_value, uint32_t cnt,
            int32_t k_value,
            const std::vector< std::vector<one_sparse_vector> > & sketchs,
            const std::vector< hash_k > & hashes)
            : m_cnt(cnt), m_size(size), m_s_value(s_value), m_k_value(k_value),
            m_sketchs(sketchs), m_hashes(hashes)
        {
        }

    private:
        uint32_t m_cnt;
        uint32_t m_size;
        int32_t m_s_value;
        int32_t m_k_value;

        std::vector< std::vector<one_sparse_vector> > m_sketchs;
        std::vector< hash_k > m_hashes;
    };

    class main_vector
    {
    public:
        main_vector()
            : m_s_value(0), m_k_value(0), m_size(0)
        {
        }

        explicit main_vector(uint32_t size, double delta)
            : m_s_value(3 * (1 - int32_t(std::ceil(std::log2(delta / 2.))))),
            m_k_value(1 + int32_t(std::ceil(std::log(size)))),
            m_size(size), m_hash(hash_k(std::pow(size, 3), m_s_value))
        {
            double delta_decode = delta / 2.;

            for (auto i = 0; i < m_k_value; ++i)
            {
                m_sketchs.push_back(
                    s_sparse_vector(size, m_s_value, delta_decode));
            }
        }

        main_vector(const main_vector & other)
        {
            m_s_value = other.m_s_value;
            m_k_value = other.m_k_value;
            m_size = other.m_size;
            m_hash = other.m_hash;
            m_sketchs = other.m_sketchs;
        }

        main_vector & operator=(const main_vector & other)
        {
            m_s_value = other.m_s_value;
            m_k_value = other.m_k_value;
            m_size = other.m_size;
            m_hash = other.m_hash;
            m_sketchs = other.m_sketchs;

            return *this;
        }

        main_vector(main_vector && other) noexcept
        {
            std::swap(m_s_value, other.m_s_value);
            std::swap(m_k_value, other.m_k_value);
            std::swap(m_size, other.m_size);
            std::swap(m_hash, other.m_hash);
            std::swap(m_sketchs, other.m_sketchs);
        }

        main_vector & operator=(main_vector && other) noexcept
        {
            std::swap(m_s_value, other.m_s_value);
            std::swap(m_k_value, other.m_k_value);
            std::swap(m_size, other.m_size);
            std::swap(m_hash, other.m_hash);
            std::swap(m_sketchs, other.m_sketchs);

            return *this;
        }

        void update(uint32_t index, int32_t value)
        {
            for (auto i = 0; i < m_k_value; ++i)
            {
                uint32_t pow = std::pow(2, i);
                if (m_hash.at(index) % pow == 0)
                {
                    m_sketchs[i].update(index, value);
                }
            }
        }

        std::pair<uint32_t, int32_t> sample()
        {
            for (auto ii = 0; ii < m_k_value; ++ii)
            {
                auto result = m_sketchs[m_k_value - 1 - ii].recover();

                if (!result.empty())
                {
                    std::uniform_int_distribution<uint32_t> rand_uint32_t(0, result.size() - 1);

                    return result[rand_uint32_t(mt)];
                }
            }

            return std::make_pair(0, 0);
        }

        friend main_vector operator+(
            const main_vector & base,
            const main_vector & other)
        {
            std::vector< s_sparse_vector > sketchs;

            for (auto i = 0; i < base.m_k_value; ++i)
            {
                sketchs.push_back(base.m_sketchs[i] + other.m_sketchs[i]);
            }

            return main_vector(base.m_size, base.m_s_value, base.m_k_value,
                base.m_hash, sketchs);
        }

    private:
        explicit main_vector(uint32_t size, int32_t s_value,
            int32_t k_value, const hash_k & hash,
            const std::vector< s_sparse_vector > & sketchs)
            : m_s_value(s_value), m_k_value(k_value), m_size(size),
            m_hash(hash), m_sketchs(sketchs)
        {
        }

    private:
        int32_t m_s_value;
        int32_t m_k_value;
        uint32_t m_size;
        hash_k m_hash;
        std::vector< s_sparse_vector > m_sketchs;
    };
}

class dsu
{
public:
    explicit dsu(uint32_t vertex_count)
        : m_vertex_count(vertex_count)
    {
        for (auto i = 0u; i < m_vertex_count; ++i)
        {
            m_rank.push_back(0);
            m_parent.push_back(i);
        }
    }

    uint32_t find(uint32_t u)
    {
        if (m_parent.at(u) == u)
        {
            return u;
        }

        m_parent[u] = find(m_parent.at(u));

        return m_parent.at(u);
    }

    void union_(uint32_t u, uint32_t v)
    {
        u = find(u);
        v = find(v);

        if (m_rank.at(u) > m_rank.at(v))
        {
            std::swap(u, v);
        }

        if (m_rank.at(u) == m_rank.at(v))
        {
            ++m_rank[v];
        }

        m_parent[u] = v;
    }

private:
    uint32_t m_vertex_count;
    std::vector<int32_t> m_rank;
    std::vector<uint32_t> m_parent;
};

constexpr double delta_const = 0.01;

class DynamicGraph
{
public:
    explicit DynamicGraph(uint32_t vertex_count)
        : m_vertex_count(vertex_count),
        m_sketch_count(1 + std::ceil(std::log2(vertex_count)))
    {
        uint32_t pow = std::pow(m_vertex_count, 2);
        for (auto i = 0; i < m_sketch_count; ++i)
        {
            m_sketch.push_back(
                generate_graph_sketch(m_vertex_count, pow, delta_const));
        }
    }

    void AddEdge(uint32_t u, uint32_t v)
    {
        uint32_t edge_number = u * m_vertex_count + v;

        for (auto i = 0; i < m_sketch_count; ++i)
        {
            m_sketch[i][u].update(edge_number, +1);
            m_sketch[i][v].update(edge_number, -1);
        }
    }

    void RemoveEdge(uint32_t u, uint32_t v)
    {
        uint32_t edge_number = u * m_vertex_count + v;

        for (auto i = 0; i < m_sketch_count; ++i)
        {
            m_sketch[i][u].update(edge_number, -1);
            m_sketch[i][v].update(edge_number, +1);
        }
    }

    uint64_t GetComponentsNumber() const
    {
        rbtree::tree< uint32_t, std::vector<uint32_t> > cur_cc;

        for (auto i = 0u; i < m_vertex_count; ++i)
        {
            cur_cc.push(i, std::vector<uint32_t>(1, i));
        }

        dsu _dsu(m_vertex_count);

        for (auto lev = 0; lev < m_sketch_count; ++lev)
        {
            rbtree::tree<uint32_t, l0sample::main_vector> sketch_sum;

            for (auto it = cur_cc.items.begin(); it != cur_cc.items.end(); ++it)
            {
                auto & key = (*it)->info.first;
                auto & component = (*it)->info.second;

                auto & sketch_sum_key = sketch_sum[key];

                sketch_sum_key = m_sketch[lev][component[0]];

                for (auto j = 1; j < component.size(); ++j)
                {
                    sketch_sum_key = sketch_sum_key + m_sketch[lev][component[j]];
                }
            }

            for (auto it = sketch_sum.items.begin(); it != sketch_sum.items.end(); ++it)
            {
                auto pair = (*it)->info.second.sample();

                if (pair.first != 0 && pair.second != 0)
                {
                    _dsu.union_(pair.first / m_vertex_count,
                        pair.first % m_vertex_count);
                }
            }

            cur_cc.clear();

            for (auto i = 0u; i < m_vertex_count; ++i)
            {
                auto parent = _dsu.find(i);

                cur_cc[parent].push_back(i);
            }
        }

        return cur_cc.size();
    }

private:
    std::vector< l0sample::main_vector >
        generate_graph_sketch(uint32_t size, uint32_t msize, double delta)
    {
        std::vector< l0sample::main_vector > result;

        for (auto i = 0; i < size; ++i)
        {
            result.push_back(l0sample::main_vector(msize, delta));
        }

        return result;
    }

private:
    const uint32_t m_vertex_count;
    const uint32_t m_sketch_count;

    std::vector< std::vector<l0sample::main_vector> > m_sketch;
};

