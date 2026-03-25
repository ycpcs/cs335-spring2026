insert into employee (Name, EID, Password, Salary, SSN) values ('Bilbo Baggins', 'EID1000', 'p@$$', 65000, '123456789');

insert into employee (Name, EID, Password, Salary, SSN) values ('Frodo Baggins', 'EID2000', 'r1ng', 50000, '098765432');
insert into employee (Name, EID, Password, Salary, SSN) values ('Sam Gamgee', 'EID3000', 'shir3', 45000, '123321123');
insert into employee (Name, EID, Password, Salary, SSN) values ('Smeagol', 'EID0001', 'pr3$$', 1000, '111222333');
insert into employee (Name, EID, Password, Salary, SSN) values ('Mithrandir', 'EID0000', 'gr@y', 999999, '111111111');   


# To use a script
Insert Employees: save as /employees.sql
Run with ```mysql> source /employees.sql```