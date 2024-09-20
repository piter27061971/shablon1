#include <map>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

struct SqlQuery {

    std::vector<std::string> column;
    std::string from;
    std::map<std::string, std::string> where;

};

class SqlSelectQueryBuilder {
public:

    std::string makeQuery() {
        std::string result = getSelect() + getFrom() + getWhere() + ";";
        return result;
    }

    const std::string getSelect() {
        std::stringstream stream;
        std::string select;
        for (auto it = query.column.begin(); it != query.column.end(); it++) {
            if (it != query.column.begin()) {
                stream << ", ";
            }
            stream << *it;
        }
        if (query.column.empty()) {
            select = "SELECT * ";
        }
        else select = "SELECT " + stream.str() + " ";
        return select;
    }

    const std::string getFrom() {
        std::string from;
        if (!query.from.empty()) {
            from = "FROM " + query.from + " ";
        }
        else from = "";
        return from;
    }

    const std::string getWhere() {
        std::stringstream stream;
        std::string where;
        for (auto it = query.where.begin(); it != query.where.end(); it++) {
            if (it != query.where.begin()) {
                stream << " AND ";
            }
            stream << it->first + "=" + it->second;
            where = "WHERE " + stream.str();
        }
        return where;
    }

    SqlSelectQueryBuilder& AddColumn(const std::string& column) noexcept {
        query.column.push_back(column);
        return *this;
    }

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& column) noexcept {
        query.column = column;
        return *this;
    }

    SqlSelectQueryBuilder& AddFrom(const std::string& table) noexcept {
        query.from = table;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value) noexcept {
        query.where[key] = value;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        query.where = kv;
        return *this;
    }

    ~SqlSelectQueryBuilder() = default;

private:
    SqlQuery query;
};

int main(int argc, char** argv) {
    SqlSelectQueryBuilder query_builder;
    auto query = query_builder.makeQuery();
    std::cout << query_builder.makeQuery() << "\n";
    assert(query_builder.makeQuery().find("SELECT *") == 0);
    assert(query_builder.makeQuery().rfind(";") == (query.size() - 1));
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");
    query_builder.AddFrom("students");
    std::cout << query_builder.makeQuery() << "\n";
    assert(query_builder.makeQuery() == "SELECT name, phone FROM students WHERE id=42 AND name=John;");
    query_builder.AddColumns({ "age", "sex" });
    query_builder.AddWhere({ { "age", "20" }, { "sex", "male" } });
    std::cout << query_builder.makeQuery() << "\n";
    assert(query_builder.makeQuery() == "SELECT name, phone FROM students WHERE age=20 AND sex=male;");
    return 0;
}