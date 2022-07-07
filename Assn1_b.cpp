// OOP in C++ using classes, with getters and setters added
#include <iostream>
#include <vector>
#include <ctime>
//#include <bits/stdc++.h>
using namespace std;

class Book;

// Essential Functionality

class User
{

protected:
   string password;
   string name;
   long long int id; 
   char type;

public:   
   string get_password()
   {
      return password;
   }
   string get_name()
   {
      return name;
   }
   long long int get_id()
   {
      return id;
   }
   char get_type()
   {
      return type;
   }

   void set_password(string password)
   {
      this->password=password;
   }
   void set_name(string name)
   {
      this->name=name;
   }
   void set_id(long long int id)
   {
      this->id=id;
   }
   void set_type(char type)
   {
      this->type=type;
   }

   virtual bool Display_my_books()
   {
      cout<<"This will be overriden"<<endl;
      return true;
   };
   
   virtual float calculate_fine()
   {
      cout<<"This will be overriden"<<endl;
      return 0.0;
   };

   virtual bool Issue(Book* b)
   {
      cout<<"This will be overriden"<<endl;
      return true;
   };

   virtual bool Return(Book* b)
   {
      cout<<"This will be overriden"<<endl;
      return true;
   };

      virtual void print()// fortunately, we do not need to override this is the subclssses
      {
            cout << "Name: " << this->name << endl;
            cout << "ID: " << this->id << endl;

            if(this->type=='p' || this->type=='P')
            {
               cout << "Type: Professor"<<endl;
            }
            else if(this->type=='s' || this->type=='S')
            {
               cout << "Type: Student" << endl;
            }
            else if(this->type=='l' || this->type=='L')
            {
               cout << "Type: Librarian" << endl;
            }
            
            cout<<endl;
      }
};

class Book
{
   public:
      Book(string Title, string Author, long long int ISBN,string Publication)
      {
            this->Title = Title;
            this->Author = Author;
            this->ISBN = ISBN;
            this->Publication=Publication;

            this->Borrower=nullptr;
            this->Due_Date=nullptr;
      }

      bool Availability()
      {
         return (this->Borrower==nullptr);
      }

      float Return_Request(User* u)
      {
         if(this->Borrower->get_id() !=u->get_id())
         {
               return -1;
         }
         
         this->Borrower=nullptr;
         this->Due_Date=nullptr;

         float fine=5.0*(difftime(std::mktime(this->Due_Date),time(0)));
         // cout<<"Time difference"<<difftime(std::mktime(this->Due_Date),time(0))<<endl;
      
         return fine;
      }

      bool Book_Request(User* u)
      {
         if(this->Borrower!=nullptr)
         {
               if(this->Borrower->get_id()==u->get_id()) // objects created different places with the copy of attributes, so we have to match the id 
               {
                  cout<<"You already have the book."<<endl;
               }
               return false;
         }
         
         this->Borrower=u;


         std::time_t t = std::time(0);   // get time now
   
         this->Due_Date = std::localtime(&t);
         this->Due_Date->tm_mday += ((u->get_type()=='p' || u->get_type()=='P')?60:30); // 60 days for professor, 30 days for other (assuming that other possible to do this calculation is the student only
         std::mktime(this->Due_Date);

         return true; 
      }

      std::tm* Show_duedate() 
      {
         return this->Due_Date;
      }; 

      void print()
      {
            cout << "Title: " << Title << endl;
            cout << "Author: " << Author << endl;
            cout << "ISBN: " << ISBN << endl;
            cout << "Publication: " << Publication << endl;

            if(Borrower!=nullptr)
            {
               cout << "Borrower: " << Borrower->get_name() << endl;
               cout << "Due Date: " << std::asctime(this->Due_Date) << endl;
            }
            else
            {
               cout << "Not Borrowed " << "" << endl;
            }

            cout<<endl;
      }

      string get_Title()
      {
         return Title;
      }
      
      string get_Author()
      {
         return Author;
      }

      long long int get_ISBN()
      {
         return ISBN;
      }

      string get_Publication()
      {
         return Publication;
      }

      User* get_Borrower()
      {
         return Borrower;
      }

      std::tm* get_Due_Date()
      {
         return Due_Date;
      }

      void set_Title(string Title)
      {
         this->Title=Title;
      }
      
      void set_Author(string Author)
      {
         this->Author=Author;
      }

      void set_ISBN(long long int ISBN)
      {
         this->ISBN=ISBN;
      }

      void set_Publication(string Publication)
      {
         this->Publication=Publication;
      }

      void set_Borrower(User* Borrower)
      {
         this->Borrower=Borrower;
      }

      void set_Due_Date(std::tm* Due_Date)
      {
         this->Due_Date=Due_Date;
      }
   private:
      string Title;
      string Author;
      long long int ISBN;
      string Publication;

      User* Borrower;

      std::tm* Due_Date;
};

class User_database
{

   friend class Librarian;

public:
   bool Add(User* u)
   {

      // cout<<"Add function passed id is: "<<u->id<<endl;

      int ind=Search(u->get_id());
      if(ind==-1) // add only if the book is not there
      {
         List_of_Users.push_back(u);
         return true;
      }
      return false;
   }

   bool Update(User* u)
   {
      int ind=Search(u->get_id());
      if(ind!=-1)
      {
         List_of_Users[ind]->set_name(u->get_name());
         List_of_Users[ind]->set_password(u->get_password());  
         List_of_Users[ind]->set_type(u->get_type());
         return true;
      }
      return false;
   }

   bool Delete(User* u)
   {
      int ind=Search(u->get_id());
      if(ind!=-1)
      {
         List_of_Users.erase(List_of_Users.begin()+ind);
         return true;
      }
      return false;
   }

   int Search(long long int id)// Search by id
   {
      int index=-1;
      for(int i=0;i<List_of_Users.size();i++)// linear search
         {
            if(List_of_Users[i]->get_id()==id)
            {
               index=i;
               break;
            }
         }
         return index;
   }

   bool Display()
   {
      if(List_of_Users.size()==0)
      {
         cout<<"No users in the database"<<endl;
         return false;
      }

      for(int i=0;i<List_of_Users.size();i++)
      {
         List_of_Users[i]->print();
      }
      return true;
   }
   
   vector<User*> get_List_of_Users()
   {
      return List_of_Users;
   }
   
   void set_List_of_Users(vector<User*> List_of_Users)
   {
      this->List_of_Users=List_of_Users;
   }
   

private:
   vector<User*> List_of_Users;
};


class Book_database
{
   friend class Librarian;

public:
   bool Add(Book* b)
   {

      // cout<<"Add function passed id is: "<<u->id<<endl;

      int ind=Search(b->get_ISBN()); // only if the isbn of a book exists
      if(ind==-1)
      {
         List_of_Books.push_back(b);
         return true;
      }
      return false;
   }

   bool Update(Book* b)
   {
      int ind=Search(b->get_ISBN());
      if(ind!=-1)
      {
         List_of_Books[ind]->set_Author(b->get_Author());
         List_of_Books[ind]->set_ISBN(b->get_ISBN());
         List_of_Books[ind]->set_Publication(b->get_Publication());

         List_of_Books[ind]->set_Borrower(b->get_Borrower());
         List_of_Books[ind]->set_Due_Date(b->get_Due_Date());
         return true;
      }
      return false;
   }

   bool Delete(Book* b)
   {
      int ind=Search(b->get_ISBN());
      if(ind!=-1)
      {
         List_of_Books.erase(List_of_Books.begin()+ind);
         return true;
      }
      return false;
   }

   int Search(long long int isbn)// Search by id
   {
      int index=-1;
      for(int i=0;i<List_of_Books.size();i++)// linear search
         {
            if(List_of_Books[i]->get_ISBN() ==isbn)
            {
               index=i;
               break;
            }
         }
         return index;
   }

   bool Display()
   {
      if(List_of_Books.size()==0)
      {
         cout<<"No books in the database"<<endl;
         return false;
      }
      
      for(int i=0;i<List_of_Books.size();i++)
      {
         List_of_Books[i]->print();
      }
      return true;
   }

   vector<Book*> get_List_of_Books()
   {
      return List_of_Books;
   }
   
   void set_List_of_Books(vector<Book*> List_of_Books)
   {
      this->List_of_Books=List_of_Books;
   }

private:
   vector<Book*> List_of_Books;
};

class Professor: public User
{
private:
   float Fine_amount;
   float Returned_fine_amount;
   vector<Book*> List_of_Books;

public:
   Professor(string name, string password, int id,float fine)
   {
      //cout<<"Id received is: "<<id;

      this->set_name(name);
      this->set_password(password);
      this->set_id(id);
      this->set_Fine_amount(fine);
      this->set_type('p');
   }

   Professor(string name, string password, int id)
   {
      //cout<<"Id received is: "<<id;
      
      this->set_name(name);
      this->set_password(password);
      this->set_id(id);
      this->set_Fine_amount(0.0);
      this->set_type('p');
   }
   
   float calculate_fine() // consists of static and dynamic parts of calculatoin
   {
      float fine=0.0,temp=0.0;
      for(int i=0;i<List_of_Books.size();i++)// linear search
      {
         temp=difftime(std::mktime(List_of_Books[i]->get_Due_Date()),time(0))/86400.00;
         fine+=5.0*((temp>0)?0:temp);
      }
      
      Fine_amount=Returned_fine_amount + fine;
      return fine;
   }

   bool Clear_fine_amount() // needed as we might have previously define fines
   {
      Fine_amount=0;
      Returned_fine_amount=0;

      return true;
   }

   bool Display_my_books()
   {
      //cout<<"Test 2 - inside my display_my_books()"<<endl;
      //cout<<"List of Books: "<<List_of_Books.size()<<endl;
      if(List_of_Books.size()==0)
      {
         cout<<"No books issued to you"<<endl;
         return false;
      }
      
      for(int i=0;i<List_of_Books.size();i++)
      {
         List_of_Books[i]->print();
      }
      return true;
   }

   bool Check_if_available(Book* b)
   {
      return (b->get_Borrower()==nullptr);
   }

   bool Issue(Book* b)
   {
         bool status = b->Book_Request(this);// check is already implemented in Book
         if(status)
         {
            List_of_Books.push_back(b);
            return true;
         }
         return false;
   }
   
   bool Return(Book* b)
   {
         float fine = b->Return_Request(this);// check is already implemented in Book
         if(fine<0)
            return false;

         this->Returned_fine_amount=fine;
         
         int ind=Search_my_books(b->get_ISBN());
         if(ind!=-1)
         {
            List_of_Books.erase(List_of_Books.begin()+ind);
            return true;
         }
         return true;
   }

   int Search_my_books(int ISBN)// Search by ISBN
   {
      int index=-1;
      for(int i=0;i<List_of_Books.size();i++)// linear search
         {
            if(List_of_Books[i]->get_ISBN()==ISBN)
            {
               index=i;
               break;
            }
         }
         return index;
   }

   float get_Fine_amount()
   {
      return Fine_amount;
   }
   float get_Returned_fine_amount()
   {
      return Returned_fine_amount;
   }
   vector<Book*> get_List_of_Books()
   {
      return List_of_Books;
   }

   void set_Fine_amount(float Fine_amount)
   {
      this->Fine_amount=Fine_amount;
   }
   void set_Returned_fine_amount(float Returned_fine_amount)
   {
      this->Returned_fine_amount=Returned_fine_amount;
   }
   void set_List_of_Books(vector<Book*> List_of_Books)
   {
      this->List_of_Books=List_of_Books;
   }
};

class Student: public User
{
private:
   float Fine_amount;
   float Returned_fine_amount;
   vector<Book*> List_of_Books;

public:
   Student(string name, string password, int id,float fine)
   {
      //cout<<"Id received is: "<<id;

      this->set_name(name);
      this->set_password(password);
      this->set_id(id);
      this->set_Fine_amount(fine);
      this->set_type('s');
   }

   Student(string name, string password, int id)
   {
      //cout<<"Id received is: "<<id;
      
      this->set_name(name);
      this->set_password(password);
      this->set_id(id);
      this->set_Fine_amount(0.0);
      this->set_type('s');
   }
   
   float calculate_fine() // consists of static and dynamic parts of calculatoin
   {
      float fine=0.0,temp=0.0;
      for(int i=0;i<List_of_Books.size();i++)// linear search
      {
         temp=difftime(std::mktime(List_of_Books[i]->get_Due_Date()),time(0))/86400.00;
         fine+=2.0*((temp>0)?0:temp);
      }
      
      Fine_amount=Returned_fine_amount + fine;
      return fine;
   }

   bool Clear_fine_amount() // needed as we might have previously define fines
   {
      Fine_amount=0;
      Returned_fine_amount=0;

      return true;
   }

   bool Display_my_books()
   {
      if(List_of_Books.size()==0)
      {
         cout<<"You have no books"<<endl;
         return false;
      }

      for(int i=0;i<List_of_Books.size();i++)
      {
         List_of_Books[i]->print();
      }
      return true;
   }

   bool Check_if_available(Book* b)
   {
      return (b->get_Borrower()==nullptr);
   }

   bool Issue(Book* b)
   {
      if(this->List_of_Books.size()>5)
      {
         cout<<"Can't Issue as Maximum limit of 5 books is reached."<<endl;
         return false;
      }

      bool status = b->Book_Request(this);// check is already implemented in Book
      if(status)
      {
         List_of_Books.push_back(b);
         return true;
      }
      return false;
   }
   
   bool Return(Book* b)
   {
         float fine = b->Return_Request(this);// check is already implemented in Book
         if(fine<0)
            return false;

         this->Returned_fine_amount=fine;
         
         int ind=Search_my_books(b->get_ISBN());
         if(ind!=-1)
         {
            List_of_Books.erase(List_of_Books.begin()+ind);
            return true;
         }
         return true;
   }

   int Search_my_books(int ISBN)// Search by ISBN
   {
      int index=-1;
      for(int i=0;i<List_of_Books.size();i++)// linear search
         {
            if(List_of_Books[i]->get_ISBN()==ISBN)
            {
               index=i;
               break;
            }
         }
         return index;
   }

   float get_Fine_amount()
   {
      return Fine_amount;
   }
   float get_Returned_fine_amount()
   {
      return Returned_fine_amount;
   }
   vector<Book*> get_List_of_Books()
   {
      return List_of_Books;
   }
   void set_Fine_amount(float Fine_amount)
   {
      this->Fine_amount=Fine_amount;
   }
   void set_Returned_fine_amount(float Returned_fine_amount)
   {
      this->Returned_fine_amount=Returned_fine_amount;
   }
   void set_List_of_Books(vector<Book*> List_of_Books)
   {
      this->List_of_Books=List_of_Books;
   }
};

class Librarian: public User
{
public:
   Librarian(string name, string password, int id)
   {
      this->name=name;
      this->password=password;
      this->id=id; 
      this->type='l';
   }
};


//Runners:

User_database* create_user_database()
{
   int n;
   cout<<"Enter the number of users: ";
   cin>>n;
   
   User_database* udb = new User_database();
   
   // Auxillary variables
   char type;
   string nm,pw;
   int id;
   User* u;

   for(int i =0;i<n;i++)
   {
      cout<< "USER - "<<(i+1)<<endl<<"Enter the type of User(p/s/l): "<<endl;
      cin>>type;
      cout<<"Enter the name of the User: ";
      cin>>nm;
      cout<<"Enter the password of the User: ";
      cin>>pw;
      cout<<"Enter the id of the User: ";
      cin>>id;

      if(type=='p' || type=='P')
      {
         u=(new Professor(nm,pw,id));
      }
      else if(type=='s' || type=='S')
      {
         u=(new Student(nm,pw,id));
      }
      else if(type=='l' || type=='L')
      {
         u=(new Librarian(nm,pw,id));
      }
      else
      {
         cout<<"Re-enter user with the type defined :)"<<endl;
         i--;
         continue;
      }

      bool status = udb->Add(u);
      if(status)
         cout<<"User added successfully."<<endl;
      else
         cout<<"User with this ID already exists."<<endl;  
   }
   
   return udb;
}

Book_database* create_book_database()
{
   int n;
   cout<<"Enter the number of books: ";
   cin>>n;
   
   Book_database* bdb=new Book_database();
   
   // Auxillary variables
   string ttl,auth,pub;
   int isbn;
   Book* b;

   // string nm,pw;
   // int id;

   for(int i =0;i<n;i++)
   {
      cout<< "BOOK - "<<(i+1)<<endl;
      cout<<"Enter the Title of the Book: ";
      cin>>ttl;
      cout<<"Enter the Author of the Book: ";
      cin>>auth;
      cout<<"Enter the ISBN of the Book: ";
      cin>>isbn;
      cout<<"Enter the Publisher of the Book: ";
      cin>>pub;

      b= (new Book(ttl,auth,isbn,pub));

      bool status = bdb->Add(b);
      if(status)
         cout<<"Book \""<<b->get_Title()<<"\" added successfully."<<endl;
      else
         cout<<"Book with this ISBN already exists."<<endl;
   }
   
   return bdb;
}

void login_and_prompt(Book_database* bdb, User_database* udb)
{    
      char selection='1';
      int id,ind,isbn,indb,uind,bind;
      string pw;
      char choice,contin;
      string ttl,auth,pub;

      char type;
      string nm;
      Book* b;
      User* u;
      bool status;

      do
      {  
         cout<<"LOGIN : "<<endl;
         cout<<"Enter the User id: ";
         cin>>id;
         

         ind=udb->Search(id);
         
         if(ind!=-1)
         {
            cout<< "Enter the password: ";
            cin>>pw;

            if(udb->get_List_of_Users()[ind]->get_password().compare(pw)==0)
            {
               cout<<"Welcome "<<udb->get_List_of_Users()[ind]->get_name()<<endl;

               if(udb->get_List_of_Users()[ind]->get_type()=='p')
               {
                  do
                  {
                     cout<<"MENU:"<<endl;
                     cout<<"a. See all the books"<<endl;
                     cout<<"b. See the books that I have"<<endl;
                     cout<<"c. Check if a book is available for Borrow"<<endl;
                     cout<<"d. Issue a book"<<endl;
                     cout<<"e. Return a book"<<endl;

                     cout<<"Enter your choice: ";
                     cin>>choice;

                     switch(choice)
                     {
                        case 'a':
                        case 'A':
                           bdb->Display();
                           break;
                        case 'b':
                        case 'B':
                           udb->get_List_of_Users()[ind]->Display_my_books();
                           break;
                        case 'c':
                        case 'C':
                           cout<<"Enter the ISBN of the book: ";
                           cin>>isbn;

                           indb=bdb->Search(isbn);
                           if(indb!=-1)
                           {
                              if(bdb->get_List_of_Books()[indb]->Availability()==true)
                              {
                                 cout<<"The book is available for borrow"<<endl;
                              }
                              else
                              {
                                 cout<<"The book is not available for borrow"<<endl;
                              }
                           }
                           else
                           {
                              cout<<"The book is not in the database"<<endl;
                           }
                           break;
                        case 'd':
                        case 'D':
                           cout<<"Enter the ISBN of the book: ";
                           cin>>isbn;

                           indb=bdb->Search(isbn);
                           if(indb!=-1)
                           {
                              if(bdb->get_List_of_Books()[indb]->Availability()==true)
                              {
                                 udb->get_List_of_Users()[ind]->Issue(bdb->get_List_of_Books()[indb]);
                                 cout<<"The book is issued"<<endl;
                              }
                              else
                              {
                                 cout<<"The book is not available for borrow"<<endl;
                              }
                           }
                           else
                           {
                              cout<<"The book is not in the database"<<endl;
                           }
                           break;
                        case 'e':
                        case 'E':
                           cout<<"Enter the ISBN of the book: ";
                           cin>>isbn;

                           indb=bdb->Search(isbn);
                           if(indb!=-1)
                           {
                              if(bdb->get_List_of_Books()[indb]->Availability()==false)
                              {
                                 if(bdb->get_List_of_Books()[indb]->get_Borrower()==udb->get_List_of_Users()[ind])
                                 {
                                    udb->get_List_of_Users()[ind]->Return((bdb->get_List_of_Books()[indb]));
                                    cout<<"The book is returned"<<endl;
                                 }
                                 else
                                 {
                                    cout<<"You have not issued the book.";
                                 }
                              }
                              else
                              {
                                 cout<<"The book is not issued"<<endl;
                              }
                           }
                           else
                           {
                              cout<<"The book is not in the database"<<endl;
                           }
                           break;
                     }


                     cout<<"Do you want to continue \""<<udb->get_List_of_Users()[ind]->get_name()<<"\"? (yes=1,no=other int) ";
                     cin>>contin;
                     /* code */
                  }while(contin=='1');
                  
               }
               else if(udb->get_List_of_Users()[ind]->get_type()=='s')
               {
                  do
                  {
                     // show the things that I can do
                     cout<<"MENU:"<<endl;
                     cout<<"a. See all the books"<<endl;
                     cout<<"b. See the books that I have"<<endl;
                     cout<<"c. Check if a book is available for Borrow"<<endl;
                     cout<<"d. Issue a book"<<endl;
                     cout<<"e. Return a book"<<endl;

                     cout<<"Enter your choice: ";
                     cin>>choice;

                     switch(choice)
                     {
                        case 'a':
                        case 'A':
                           bdb->Display();
                           break;
                        case 'b':
                        case 'B':
                           udb->get_List_of_Users()[ind]->Display_my_books();
                           break;
                        case 'c':
                        case 'C':
                           cout<<"Enter the ISBN of the book: ";
                           cin>>isbn;

                           indb=bdb->Search(isbn);
                           if(indb!=-1)
                           {
                              if(bdb->get_List_of_Books()[indb]->Availability()==true)
                              {
                                 cout<<"The book is available for borrow"<<endl;
                              }
                              else
                              {
                                 cout<<"The book is not available for borrow"<<endl;
                              }
                           }
                           else
                           {
                              cout<<"The book is not in the database"<<endl;
                           }
                           break;
                        case 'd':
                        case 'D':
                           cout<<"Enter the ISBN of the book: ";
                           cin>>isbn;

                           indb=bdb->Search(isbn);
                           if(indb!=-1)
                           {
                              if(bdb->get_List_of_Books()[indb]->Availability()==true)
                              {
                                 udb->get_List_of_Users()[ind]->Issue(bdb->get_List_of_Books()[indb]);
                                 cout<<"The book is issued"<<endl;
                              }
                              else
                              {
                                 cout<<"The book is not available for borrow"<<endl;
                              }
                           }
                           else
                           {
                              cout<<"The book is not in the database"<<endl;
                           }
                           break;
                        case 'e':
                        case 'E':
                           cout<<"Enter the ISBN of the book: ";
                           cin>>isbn;

                           indb=bdb->Search(isbn);
                           if(indb!=-1)
                           {
                              if(bdb->get_List_of_Books()[indb]->Availability()==false)
                              {
                                 if(bdb->get_List_of_Books()[indb]->get_Borrower()==udb->get_List_of_Users()[ind])
                                 {
                                    udb->get_List_of_Users()[ind]->Return((bdb->get_List_of_Books()[indb]));
                                    cout<<"The book is returned"<<endl;
                                 }
                                 else
                                 {
                                    cout<<"You have not issued the book.";
                                 }
                              }
                              else
                              {
                                 cout<<"The book is not issued"<<endl;
                              }
                           }
                           else
                           {
                              cout<<"The book is not in the database"<<endl;
                           }
                           break;
                     }


                     cout<<"Do you want to continue \""<<udb->get_List_of_Users()[ind]->get_name()<<"\"? (yes=1,no=other int) ";
                     cin>>contin;
                     /* code */
                  }while(contin=='1');
                  
               }
               else if(udb->get_List_of_Users()[ind]->get_type()=='l')
               {
                  cout<<"Inside librarian"<<endl;
                  do
                  {
                     // show the things that I can do
                     cout<<"MENU:"<<endl;
                     cout<<"a. Add a book"<<endl;
                     cout<<"b. Update a book"<<endl;
                     cout<<"c. Delete a book"<<endl;
                     cout<<"d. Add a User"<<endl;
                     cout<<"e. Update a User"<<endl;
                     cout<<"f. Delete a User"<<endl;
                     cout<<"g. Show all the books"<<endl;
                     cout<<"h. Show all the Users"<<endl;
                     cout<<"i. Check books issued to a user"<<endl;
                     cout<<"j. Check the borrower of a Book"<<endl;
                     cout<<"k. Check fine of a user"<<endl;
                     
                     cout<<"Enter your choice: ";
                     cin>>choice;

                     switch(choice)
                     {
                           
                        case 'a':
                        case 'A':
                           cout<<"Enter the Title of the Book: ";
                           cin>>ttl;
                           cout<<"Enter the Author of the Book: ";
                           cin>>auth;
                           cout<<"Enter the ISBN of the Book: ";
                           cin>>isbn;
                           cout<<"Enter the Publisher of the Book: ";
                           cin>>pub;
                           
                           b= (new Book(ttl,auth,isbn,pub));

                           try
                           {
                              status = bdb->Add(b);
                              if(status)
                                 cout<<"Book \""<<b->get_Title()<<"\" added successfully"<<endl;
                              else
                                 cout<<"Book with the same ISBN already exists."<<endl;
                           }
                           catch(const std::exception& e)
                           {
                              std::cerr << e.what() << '\n';
                           }
                           break;

                        case 'b':
                        case 'B':
                           cout<<"Enter the ISBN of the Book: ";
                           cin>>isbn;
                           cout<<"Enter the Title of the Book: ";
                           cin>>ttl;
                           cout<<"Enter the Author of the Book: ";
                           cin>>auth;                           
                           cout<<"Enter the Publisher of the Book: ";
                           cin>>pub;

                           b= (new Book(ttl,auth,isbn,pub));

                           try
                           {
                              status= bdb->Update(b);
                              if(status)
                                 cout<<"Book Updated Successfully"<<endl;
                              else
                                 cout<<"Book not found in the database"<<endl;
                           }
                           catch(const std::exception& e)
                           {
                              std::cerr << e.what() << '\n';
                           }

                           break;

                        case 'c':
                        case 'C':
                           cout<<"Enter the ISBN of the book to be deleted: ";
                           cin>>isbn;
                           
                           status= bdb->Delete(b);
                           if(status)
                              cout<<"Book Deleted Successfully"<<endl;
                           else
                              cout<<"Book not found in the database"<<endl;

                           break;

                        case 'd':
                        case 'D':
                           User* u;

                           cout<< "Enter the type of User: "<<endl;
                           cin>>type;
                           cout<<"Enter the name of the User: ";
                           cin>>nm;
                           cout<<"Enter the password of the User: ";
                           cin>>pw;
                           cout<<"Enter the id of the User: ";
                           cin>>id;

                           if(type=='p' || type=='P')
                           {
                              u=(new Professor(nm,pw,id));
                           }
                           else if(type=='s' || type=='S')
                           {
                              u=(new Student(nm,pw,id));
                           }
                           else if(type=='l' || type=='L')
                           {
                              u=(new Librarian(nm,pw,id));
                           }
                           else
                           {
                              cout<<"Invalid User type"<<endl;
                           }
                        
                           try
                           {
                              status= udb->Add(u);
                              if(status)
                                 cout<<"User added successfully"<<endl;
                              else
                                 cout<<"User with same ID already exists."<<endl;
                           }
                           catch(const std::exception& e)
                           {
                              std::cerr << e.what() << '\n';
                           }
                           break;

                        case 'e':
                        case 'E':
                           
                           cout<<"Enter the id of the User: ";
                           cin>>id;
                           cout<< "Enter the type of User: "<<endl;
                           cin>>type;
                           cout<<"Enter the name of the User: ";
                           cin>>nm;
                           cout<<"Enter the password of the User: ";
                           cin>>pw;

                           if(type=='p' || type=='P')
                           {
                              u=(new Professor(nm,pw,id));
                           }
                           else if(type=='s' || type=='S')
                           {
                              u=(new Student(nm,pw,id));
                           }
                           else if(type=='l' || type=='L')
                           {
                              u=(new Librarian(nm,pw,id));
                           }
                           else
                           {
                              cout<<"Invalid User type"<<endl;
                           }
                        
                           try
                           {
                              status= udb->Update(u);
                              if(status)
                                 cout<<"User updated successfully"<<endl;
                              else
                                 cout<<"User not found in database"<<endl;
                           }
                           catch(const std::exception& e)
                           {
                              std::cerr << e.what() << '\n';
                           }
                           break;
                        case 'f':
                        case 'F':
                           cout<<"Enter the id of the user to be deleted: ";
                           cin>>id;

                           status= bdb->Update(b);
                           if(status)
                              cout<<"User Deleted Successfully"<<endl;
                           else
                              cout<<"User not found in the database"<<endl;
                           break;
                        case 'g':
                        case 'G':
                           status = bdb->Display();
                           if(!status)
                              cout<<"No books in the database"<<endl;
                           break;
                        case 'h':
                        case 'H':
                           status = udb->Display();
                           if(!status)
                              cout<<"No user in the database"<<endl;
                           break;
                        case 'i':
                        case 'I':
                           cout<<"Enter the id of the user:"<<endl;
                           cin>>id;
                           
                           uind=udb->Search(id);
                           if(uind!=-1)
                           {
                              udb->get_List_of_Users()[uind]->Display_my_books();
                           }
                           else
                           {
                              cout<<"The User does not exist.";
                           }
                           break;
                        case 'j':
                        case 'J':
                           cout<<"Enter the isbn of the book:"<<endl;
                           cin>>isbn;
                           
                           bind=bdb->Search(isbn);
                           if(bind!=-1)
                           {
                              cout<<"The Borrower is \""<<bdb->get_List_of_Books()[bind]->get_Borrower()->get_name()<<"\""<<endl ;
                           }
                           else
                           {
                              cout<<"The Book does not exist.";
                           }
                           break;
                        case 'k':
                        case 'K':
                           cout<<"Enter the id of the user:"<<endl;
                           cin>>id;
                           
                           uind=udb->Search(id);
                           if(uind!=-1)
                           {
                              cout<<"Fine = "<<udb->get_List_of_Users()[uind]->calculate_fine()<<endl;
                           }
                           else
                           {
                              cout<<"The User does not exist.";
                           }
                           break;
                     }


                     cout<<"Do you want to continue? (yes=1,no=other int) ";
                     cin>>contin;
                  }while(contin=='1');
                  
               }
            }
            else
            {
               cout<<"Wrong password"<<endl;
            }

         }
         else
         {
            cout<<"User ID does not exist"<<endl;
         }
         
         cout<<"Do you want to login in and continue (yes=1/no=other integer)? ";
         cin>>selection;
         
      }while(selection=='1');
}

int main()
{   
   Book_database* bdb = create_book_database();
   User_database* udb = create_user_database();
   login_and_prompt(bdb,udb);

   return 0;
}