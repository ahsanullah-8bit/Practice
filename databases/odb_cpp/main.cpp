#include <iostream>
#include <memory>

#include <odb/database.hxx>
#include <odb/transaction.hxx>
#include <odb/schema-catalog.hxx>
#include <odb/sqlite/database.hxx>

#include "models-odb.hxx"

void manage_schema(odb::database& db);
void run_crud_demo(odb::database& db);
void add_data(odb::database& db);

int main() {

  try
  {
    auto db = std::make_shared<odb::sqlite::database>("odb_cpp.db", SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
    
    manage_schema(*db);
    // run_crud_demo(*db);
    // add_data(*db);
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}

void manage_schema(odb::database& db)
{
  auto current_v = odb::schema_catalog::current_version(db);
  auto db_v = db.schema_version();

  if (db_v == 0) {
    std::cout << "Database empty. Creating schema version " << current_v << "...\n";

    odb::transaction t (db.begin ());
    odb::schema_catalog::create_schema(db);
    t.commit();
  } 
  else if (db_v < current_v) {
    std::cout << "Migrating database from version " << db_v << " to " << current_v << "...\n";
    odb::transaction t (db.begin ());
    odb::schema_catalog::migrate_schema(db, current_v);
    t.commit();
  } else {
        std::cout << "Database is up to date (Version " << db_v << ").\n";
  }
}

void run_crud_demo(odb::database& db) {
    try {
        // CREATE
        unsigned long tid;
        {
            odb::transaction t(db.begin());
            Task myTask("Learn ODB", 5);
            tid = db.persist(myTask);
            t.commit();
            std::cout << "Created Task ID: " << tid << "\n";
        }

        // UPDATE
        {
            odb::transaction t(db.begin());
            std::unique_ptr<Task> task(db.load<Task>(tid));
            task->description = "Master ODB";
            task->priority = 1; // Highest priority
            db.update(*task);
            t.commit();
            std::cout << "Updated Task " << tid << " description to " << task->description << "\n";
        }

        // DELETE
        {
            odb::transaction t(db.begin());
            db.erase<Task>(tid);
            t.commit();
            std::cout << "Deleted Task " << tid << "\n";
        }
    } 
    catch (const odb::exception& e) {
        std::cerr << "Database error: " << e.what() << std::endl;
    }
}

void add_data(odb::database &db)
{
  try {
    unsigned long tid;
    {
        odb::transaction t(db.begin());
        std::vector<Task> tasks = {
            Task{"Learn ODB", 1},
            Task{"Build a C++ application with ODB", 2},
            Task{"Master C++ ORM with ODB", 3}
        };

        for(auto& task : tasks) {
          tid = db.persist(task);
          std::cout << "Created Task ID: " << tid << ", Description: " << task.description << "\n";
        }
        
        std::vector<User> users = {
            User{"Alice"},  
            User{"Bob"},
            User{"Charlie"}
        };

        for (auto& user : users) {
          tid = db.persist(user);
          std::cout << "Created User ID: " << tid << ", Name: " << user.name << "\n";
        }
        t.commit();
    }
  } 
  catch (const odb::exception& e) {
      std::cerr << "Database error: " << e.what() << std::endl;
  }
}
