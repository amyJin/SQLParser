#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_no_skip.hpp>

#include <string>

namespace ensighten {
  
  namespace qi = boost::spirit::qi;
  namespace ascii = boost::spirit::ascii;

  /*
   * our select parser
   */
  template <typename Iterator>
  struct sqlparser: qi::grammar<Iterator, ascii::space_type> 
  {
  
    sqlparser();

    qi::rule<Iterator, ascii::space_type> keywords;
    qi::rule<Iterator, ascii::space_type> select;
    qi::rule<Iterator, ascii::space_type> term;
    qi::rule<Iterator, ascii::space_type> selectExpression;
    qi::rule<Iterator, ascii::space_type> tableExpression;
    qi::rule<Iterator, ascii::space_type> expression;
    qi::rule<Iterator, ascii::space_type> order;
    qi::rule<Iterator, ascii::space_type> value;
    qi::rule<Iterator, ascii::space_type> columnName;
    qi::rule<Iterator, ascii::space_type> Function;
    qi::rule<Iterator, ascii::space_type> Case;
    qi::rule<Iterator, ascii::space_type> CaseWhen;
    qi::rule<Iterator, ascii::space_type> tableAlias;
    qi::rule<Iterator, ascii::space_type> string;
    qi::rule<Iterator, ascii::space_type> name;
    qi::rule<Iterator, ascii::space_type> count;
    qi::rule<Iterator, ascii::space_type> concat;
    qi::rule<Iterator, ascii::space_type> dateFormat;
    qi::rule<Iterator, ascii::space_type> dateAdd;
    qi::rule<Iterator, ascii::space_type> dateUnit;
    qi::rule<Iterator, ascii::space_type> andCondition;
    qi::rule<Iterator, ascii::space_type> condition;
    qi::rule<Iterator, ascii::space_type> operand;
    qi::rule<Iterator, ascii::space_type> conditionRightHandSide;
    qi::rule<Iterator, ascii::space_type> summand;
    qi::rule<Iterator, ascii::space_type> factor;
    qi::rule<Iterator, ascii::space_type> compare;
    qi::rule<Iterator, ascii::space_type> columnAlias;
    qi::rule<Iterator, ascii::space_type> schemaName;
    qi::rule<Iterator, ascii::space_type> tableName;
    qi::rule<Iterator, ascii::space_type> newTableAlias;
    qi::rule<Iterator, ascii::space_type> rowCountInt;
    qi::rule<Iterator, ascii::space_type> distinct;
    qi::rule<Iterator, ascii::space_type> sum;
    qi::rule<Iterator, ascii::space_type> inet_ntoa;
    qi::rule<Iterator, ascii::space_type> inet_aton;
    
    qi::rule<Iterator, ascii::space_type> insert;
    qi::rule<Iterator, ascii::space_type> update;
    qi::rule<Iterator, ascii::space_type> Delete;
    qi::rule<Iterator, ascii::space_type> truncate;
    qi::rule<Iterator, ascii::space_type> showTableStatus;
    qi::rule<Iterator, ascii::space_type> replace;

    qi::rule<Iterator> space;
    qi::rule<Iterator, ascii::space_type> start;

#if 0


    qi::rule<Iterator, ascii::space_type> distinct;
    qi::rule<Iterator, ascii::space_type> count;

    qi::rule<Iterator, ascii::space_type> punctuate_within_exp;
    qi::rule<Iterator, ascii::space_type> concat_exp;
    qi::rule<Iterator, ascii::space_type> concat_exps;
    qi::rule<Iterator, ascii::space_type> concat;

    qi::rule<Iterator, ascii::space_type> inet_ntoa;

    qi::rule<Iterator, ascii::space_type> as;
	
    qi::rule<Iterator, ascii::space_type> space;
    qi::rule<Iterator, ascii::space_type> key;
    qi::rule<Iterator, ascii::space_type> any;
    qi::rule<Iterator, ascii::space_type> atomexp;
    qi::rule<Iterator, ascii::space_type> exp;
    qi::rule<Iterator, ascii::space_type> exps;
    qi::rule<Iterator, ascii::space_type> value;
    qi::rule<Iterator, ascii::space_type> string_;
    qi::rule<Iterator, ascii::space_type> field;
    qi::rule<Iterator, ascii::space_type> fields;
    qi::rule<Iterator, ascii::space_type> tables;
    qi::rule<Iterator, ascii::space_type> select_clause;
    qi::rule<Iterator, ascii::space_type> from_clause;
    qi::rule<Iterator, ascii::space_type> where_clause;
    qi::rule<Iterator, ascii::space_type> group_clause;
    qi::rule<Iterator, ascii::space_type> order_clause;
    qi::rule<Iterator, ascii::space_type> select_from; 
    qi::rule<Iterator, ascii::space_type> select_where_join;

    qi::rule<Iterator, ascii::space_type> insert_string_value;
    qi::rule<Iterator, ascii::space_type> insert_values;
    qi::rule<Iterator, ascii::space_type> insert_;

    qi::rule<Iterator, ascii::space_type> set_value;
    qi::rule<Iterator, ascii::space_type> set_values;
    qi::rule<Iterator, ascii::space_type> set_clause;
    qi::rule<Iterator, ascii::space_type> update_;

    qi::rule<Iterator, ascii::space_type> delete_;

    qi::rule<Iterator, ascii::space_type> join_clause;
    qi::rule<Iterator, ascii::space_type> on_clause;
    qi::rule<Iterator, ascii::space_type> join_on_clause;
    qi::rule<Iterator, ascii::space_type> join_keyword;

    qi::rule<Iterator, ascii::space_type> start;
#endif

  };
  
  /*
   * our insert parser
   
  template <typename Iterator>
  struct insertparser: qi::grammar<Iterator, ascii::space_type> 
  {
    insertparser();
  }
   */

  class Parser
  {
  private:
    Parser() {}

  public:
    static bool Parse(std::string const & str);
  };
}


