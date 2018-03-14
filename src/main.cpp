#include <iostream>
#include <string>
#include <functional>
#include <tuple>

#include "dynamic_graph.hpp"


// tests l0sample
void tests_fast_pow();
void tests_one_sparse_vector();
void tests_s_sparse_vector();
void tests_main_vector();

// tests DynamicGraph
void tests_dynamic_graph();
void hard_test();
void simple_test();

int main()
{
    // std::ios::sync_with_stdio(false);
    // std::cin.tie(nullptr);

    // tests l0sample
    // tests_fast_pow();
    // tests_one_sparse_vector();
    // tests_s_sparse_vector();
    // tests_main_vector();

    // tests DynamicGraph
    // tests_dynamic_graph();
    // hard_test(); 
    simple_test();

    return 0;
}

void simple_test()
{
    std::cout << "Simple test: \n";

    int64_t vertex_count, count_req;

    std::cin >> vertex_count;
    std::cin >> count_req;

    DynamicGraph g(vertex_count);

    for (int64_t i = 0; i < count_req; ++i)
    {
        char operation;
        std::cin >> operation;

        if (operation == '+')
        {
            int64_t u, v;

            std::cin >> u;
            std::cin >> v;

            g.AddEdge(u, v);
        }
        else if (operation == '-')
        {
            int64_t u, v;

            std::cin >> u;
            std::cin >> v;

            g.RemoveEdge(u, v);
        }
        else
        {
            std::cout << g.GetComponentsNumber() << "\n";
        }
    }
    
}

void tests_fast_pow()
{
    std::cout << "Tests function fast_pow:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        if (l0sample::fast_pow(1469, 17, 1601) == 1411709)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        if (l0sample::fast_pow(1191, 17, 1601) == 1884162)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }

    // Test 3
    {
        std::cout << "-- Test 3: ";

        if (l0sample::fast_pow(301, 17, 1601) == 87290)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }

    // Test 4
    {
        std::cout << "-- Test 4: ";

        if (l0sample::fast_pow(20000, 17, 1601) == 19700000)
        {
            std::cout << "True\n";
        }
        else
        {
            std::cout << "False";
            return;
        }
    }
}

void tests_one_sparse_vector()
{
    std::cout << "Tests one sparse vector:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        l0sample::one_sparse_vector r(100, 0.001);

        if (r.correct() != false)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        l0sample::one_sparse_vector r(100, 0.001);
        r.update(17, 42);

        if (r.correct() != true)
        {
            std::cout << "Fail 1\n";
            return;
        }

        auto pair = r.recover();

        if (pair.first != 17 || pair.second != 42)
        {
            std::cout << "Fail 2\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 3
    {
        std::cout << "-- Test 3: ";

        l0sample::one_sparse_vector r(100, 0.001);
        r.update(1, 1);
        r.update(2, 2);

        if (r.correct() != false)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 4
    {
        std::cout << "-- Test 4: ";

        l0sample::one_sparse_vector r(100, 0.001);

        for (auto i = 0u; i < 100; ++i)
        {
            r.update(i, i + 17);
        }
        r.update(17, 42);

        for (auto i = 0u; i < 100; ++i)
        {
            r.update(i, -int32_t(i + 17));
        }

        if (r.correct() != true)
        {
            std::cout << "Fail 1\n";
            return;
        }

        auto pair = r.recover();

        if (pair.first != 17 || pair.second != 42)
        {
            std::cout << "Fail 2\n";
            return;
        }

        std::cout << "True\n";
    }
}

void tests_s_sparse_vector()
{
    std::cout << "Tests s sparse vector:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        if (r.touched() != false)
        {
            std::cout << "False\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        if (!r.recover().empty())
        {
            std::cout << "False\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 3
    {
        std::cout << "-- Test 3: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        r.update(17, 42);

        std::uniform_int_distribution<int64_t> rand_int64_t_one(0, 99);
        std::uniform_int_distribution<int32_t> rand_int64_t_two(1, 25);

        for (auto i = 0u; i < 100; ++i)
        {
            auto ind = rand_int64_t_one(mt);
            auto val = rand_int64_t_two(mt);

            r.update(ind, val);
            r.update(ind, -val);
        }

        if (r.touched() != true)
        {
            std::cout << "False 1\n";
            return;
        }

        auto result = r.recover();

        if (result.size() != 1)
        {
            std::cout << "False 2\n";
            return;
        }

        if (result[0].first != 17 || result[0].second != 42)
        {
            std::cout << "False 3\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 4
    {
        std::cout << "-- Test 4: ";

        l0sample::s_sparse_vector r(100, 5, 0.01);

        std::vector< std::pair<int64_t, int64_t> > updates = {
            { 17, 42 }, { 18, 43 }, { 19, 44 }, { 20, 45 }, { 21, 46 }
        };

        for (auto & pair : updates)
        {
            r.update(pair.first, pair.second);
        }

        std::uniform_int_distribution<int64_t> rand_int64_t_one(0, 99);
        std::uniform_int_distribution<int64_t> rand_int64_t_two(1, 25);

        for (auto i = 0u; i < 100; ++i)
        {
            auto ind = rand_int64_t_one(mt);
            auto val = rand_int64_t_two(mt);

            r.update(ind, val);
            r.update(ind, -val);
        }

        auto result = r.recover();

        if (result.size() != updates.size())
        {
            std::cout << "False 1\n";
            return;
        }

        for (auto & pair : updates)
        {
            auto search = std::find(result.begin(), result.end(), pair);
            if (search == result.end())
            {
                std::cout << "Fail 2\n";
                return;
            }
        }

        std::cout << "True\n";
    }
}

void tests_main_vector()
{
    std::cout << "Tests main vector:\n";

    // Test 1
    {
        std::cout << "-- Test 1: ";

        l0sample::main_vector r(100, 0.01);

        auto result = r.sample();

        if (result.first != 0 || result.second != 0)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "-- Test 2: ";

        int64_t size = 20;

        std::vector< l0sample::main_vector > rs;

        for (auto i = 0u; i < 10; ++i)
        {
            rs.push_back(l0sample::main_vector(size, 0.01));
        }

        std::vector<int64_t> vals(size, 0);

        std::uniform_int_distribution<int64_t> rand_int64_t_one(0, size - 1);
        std::uniform_int_distribution<int64_t> rand_int64_t_two(-5, 5);

        for (auto i = 0; i < size; ++i)
        {
            auto ind = rand_int64_t_one(mt);
            auto val = rand_int64_t_two(mt);

            vals[ind] += val;

            for (auto & r : rs)
            {
                r.update(ind, val);
            }
        }

        for (auto i = 0u; i < size; ++i)
        {
            if (i % 2 == 1 && vals[i] != 0)
            {
                for (auto & r : rs)
                {
                    r.update(i, -vals[i]);
                }

                vals[i] -= vals[i];
            }
        }

        std::vector< std::pair<int64_t, int64_t> > answers;

        //std::cout << "vals:\n";
        //for (auto i = 0u; i < vals.size(); ++i)
        //{
        //    std::cout << i << "  ";
        //}
        //std::cout << "\n";
        //for (auto i = 0u; i < vals.size(); ++i)
        //{
        //    std::cout << vals[i] << ", " << std::string(i + 1 > 9 ? 1 : 0, ' ');
        //}


        //std::cout << "\nanswer:\n";
        for (auto & r : rs)
        {
            answers.push_back(r.sample());
            //std::cout << "(" << (answers.end() - 1)->first << ", "
            //    << (answers.end() - 1)->second << "), ";
        }

        //std::cout << "\n";

        for (auto a : answers)
        {
            if (a.first % 2 != 0 || a.second != vals[a.first])
            {
                std::cout << "False\n";
                return;
            }
        }

        std::cout << "True\n";
    }
}

void tests_dynamic_graph()
{
    std::cout << "Tests dynamic graph:\n";

    // Test 1
    {
        std::cout << "Test 1:\n";

        DynamicGraph g(6);

        std::vector< std::pair<int64_t, int64_t> > edges = {
            { 0, 1 },{ 1, 2 },{ 1, 3 },{ 2, 3 },{ 4, 5 }
        };

        for (auto & pair : edges)
        {
            g.AddEdge(pair.first, pair.second);
        }

        auto result = g.GetComponentsNumber();

        if (result != 2)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 2
    {
        std::cout << "Test 2:\n";
        DynamicGraph g(6);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(int64_t, int64_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);

        std::vector<std::tuple<int64_t, int64_t, decltype(add)>> edges = {
            { 4, 5, add },{ 0, 1, add },{ 1, 3, add },{ 2, 3, add },{ 1, 2, add }
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        std::cout << "Size: " << result << "\n";

        if (result != 2)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 3
    {
        std::cout << "Test 3:\n";
        DynamicGraph g(6);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(int64_t, int64_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
        std::function<void(int64_t, int64_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

        std::vector<std::tuple<int64_t, int64_t, decltype(add)>> edges = {
            { 0, 2, add },{ 0, 1, add },{ 1, 2, add },{ 0, 3, rem },
        { 1, 3, add },{ 2, 3, add },{ 1, 5, add },{ 4, 5, add },
        { 1, 5, rem }
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        if (result != 2)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 4
    {
        std::cout << "Test 4:\n";
        DynamicGraph g(6);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(int64_t, int64_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
        std::function<void(int64_t, int64_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

        std::vector<std::tuple<int64_t, int64_t, decltype(add)>> edges = {
            { 0, 2, add },{ 0, 1, add },{ 1, 2, add },{ 0, 2, rem },
        { 1, 3, add },{ 2, 3, add },{ 1, 5, add },{ 4, 5, add },
        { 1, 5, rem },{ 0, 1, rem },{ 1, 2, rem },{ 4, 5, rem },
        { 1, 3, rem },{ 2, 3, rem }
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        if (result != 6)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 5
    {
        std::cout << "Test 5:\n";
        DynamicGraph g(7);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(int64_t, int64_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
        std::function<void(int64_t, int64_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

        std::vector<std::tuple<int64_t, int64_t, decltype(add)>> edges = {
            { 0, 1, add },{ 1, 2, add },{ 2, 3, add },{ 3, 4, add },
            { 1, 3, add },{ 2, 4, add },{ 3, 5, add },{ 4, 6, add },
            { 0, 1, rem },{ 1, 2, rem },{ 2, 3, rem },{ 3, 4, rem },
            { 1, 3, rem },{ 2, 4, rem },{ 3, 5, rem },{ 4, 6, rem },
        };

        for (auto & trio : edges)
        {
            auto u = std::get<0>(trio);
            auto v = std::get<1>(trio);
            auto comd = std::get<2>(trio);

            comd(u, v);
        }

        auto result = g.GetComponentsNumber();

        if (result != 7)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 6
    {
        std::cout << "Test 6:\n";
        DynamicGraph g(0);

        auto result = g.GetComponentsNumber();

        std::cout << "Size: " << result << "\n";

        if (result != 0)
        {
            std::cout << "Fail\n";
            return;
        }

        std::cout << "True\n";
    }

    // Test 7
    {
        std::cout << "Test 7:\n";
        DynamicGraph g(7);

        using std::placeholders::_1;
        using std::placeholders::_2;

        std::function<void(int64_t, int64_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
        std::function<void(int64_t, int64_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

        std::vector<std::tuple<int64_t, int64_t, decltype(add), std::string>> edges = {
            { 0, 1, add, "Add" }, { 1, 2, add, "Add" }, { 2, 3, add, "Add" }, { 3, 4, add, "Add" },
            { 0, 1, rem, "Rem" }, { 1, 2, rem, "Rem" }, { 2, 3, rem, "Rem" }, { 3, 4, rem, "Rem" },
            { 1, 3, add, "Add" }, { 2, 4, add, "Add" }, { 3, 5, add, "Add" }, { 4, 6, add, "Add" },
            { 1, 3, rem, "Rem" }, { 2, 4, rem, "Rem" }, { 3, 5, rem, "Rem" }, { 4, 6, rem, "Rem" }
        };

        for (auto & data : edges)
        {
            auto u = std::get<0>(data);
            auto v = std::get<1>(data);
            auto comd = std::get<2>(data);

            comd(u, v);

            std::cout << std::get<3>(data) << " (" << u << ", " << v << ")";

            std::cout << " -- Component: " << g.GetComponentsNumber() << "\n";
        }
    }

    // Test 8
    {
        std::cout << "Test 8: ";

        DynamicGraph g(0);

        if (g.GetComponentsNumber() != 0)
        {
            std::cout << "false\n";
        }

        std::cout << "true\n";
    }
}


void hard_test()
{
    std::cout << "Hard test:\n";
    DynamicGraph g(7);

    using std::placeholders::_1;
    using std::placeholders::_2;

    std::function<void(int64_t, int64_t)> add = std::bind(&DynamicGraph::AddEdge, &g, _1, _2);
    std::function<void(int64_t, int64_t)> rem = std::bind(&DynamicGraph::RemoveEdge, &g, _1, _2);

    std::vector<std::tuple<int64_t, int64_t, decltype(add), std::string>> edges = {
        { 0, 1, add, "Add" }, { 1, 2, add, "Add" }, { 2, 3, add, "Add" }, { 3, 4, add, "Add" },
        { 0, 1, rem, "Rem" }, { 1, 2, rem, "Rem" }, { 2, 3, rem, "Rem" }, { 3, 4, rem, "Rem" },
        { 1, 3, add, "Add" }, { 2, 4, add, "Add" }, { 3, 5, add, "Add" }, { 4, 6, add, "Add" },
        { 1, 3, rem, "Rem" }, { 2, 4, rem, "Rem" }, { 3, 5, rem, "Rem" }, { 4, 6, rem, "Rem" }
    };

    for (auto & data : edges)
    {
        auto u = std::get<0>(data);
        auto v = std::get<1>(data);
        auto comd = std::get<2>(data);

        comd(u, v);

        auto result = g.GetComponentsNumber();

        std::cout << std::get<3>(data) << " (" << u << ", " << v << ")";

        std::cout << " -- Component: " << result << "\n";
    }
}
