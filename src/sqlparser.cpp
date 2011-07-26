#include <string>

#include "sqlparser.h"

using namespace std;

namespace ensighten {
  
namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

template<typename Iterator>
sqlparser<Iterator>::sqlparser() 
: sqlparser::base_type(start) 
{ 
  using qi::char_;
  using qi::int_;
  using qi::float_;
  using qi::double_;
  using qi::lit;
  using qi::lexeme;
  using qi::no_skip;
  using ascii::no_case;

  keywords = 
              no_case["AND"]
            | no_case["AS"]
            | no_case["ASC"]
            | no_case["BY"]
            | no_case["CASE"]
            | no_case["CONCAT"]
            | no_case["COUNT"]
            | no_case["CROSS"]
            | no_case["DATE_ADD"]
            | no_case["DATE_FORMAT"]
            | no_case["DESC"]
            | no_case["DISTINCT"]
            | no_case["ELSE"]
            | no_case["FOR UPDATE"]
            | no_case["FROM"]
            | no_case["GROUP"]
            | no_case["IN"]
            | no_case["IS"]
            | no_case["INNER"]
            | no_case["INET_NTOA"]
            | no_case["INET_ATON"]
	    | no_case["INSERT"]
            | no_case["INTO"]
            | no_case["LEFT"]
            | no_case["LIMIT"]
            | no_case["NATURAL"]
            | no_case["NOT"]
            | no_case["NULL"]
            | no_case["OFFSET"]
            | no_case["ORDER"]
            | no_case["OR"]
            | no_case["ON"]
            | no_case["OUTER"]
            | no_case["RIGHT"]
            | no_case["SAMPLE_SIZE"]
            | no_case["SELECT"]
            | no_case["SET"]
            | no_case["SUM"]
            | no_case["THEN"]
            | no_case["UPDATE"]   
            | no_case["VALUES"]
            | no_case["WHERE"]
            | no_case["WHEN"]
            ;

  name =   lexeme[char_("a-zA-Z_") >> *(char_("a-zA-Z_0-9"))] 
         - keywords ;

  columnName    = name;
  columnAlias   = name;
  tableName     = name;
  tableAlias    = name;
  newTableAlias = name;
  schemaName    = name;

  rowCountInt = int_;

  string =    lit("'")
           >> *(lit("\\\'") | lit("\\\\") | lit("\\\""))
           >> -( +(char_ - '"' - '\\' - "'") % ( lit("\\\"") | lit("\\\'") | lit("\\\\")))
           >> *(lit("\\\'") | lit("\\\\") | lit("\\\"")) 
           >> lit("'")
           ;

  space  = lit(' ') | lit('\t') | lit('\n');

  value =   string
          | (int_ >> ',' >> int_)
          | int_
          | double_
          | no_case["NULL"]
          ;

  // A value. Parameters can be indexed, for example 
  // ?1 meaning the first parameter. 
  term =   ( Case                                 )
         | ( CaseWhen                             )
         | ( value                                )
         | ( tableAlias >> lit('.') >> columnName )
         | ( columnName                           )
         | ( lit('?') >> int_                     )
         | ( Function                             )
         | ( (lit('-') | lit('+')) >> term        )
         | ( lit('(') >> expression >> lit(')')   )
         | ( select                               )
         ;


  // Functions:
  // count, concat, dateFormat
  Function =   count
             | concat
             | dateFormat
             | dateAdd
             | distinct
             | sum
             | inet_ntoa
             | inet_aton
             ;

  count =    no_case["COUNT"] >> '('
          >> ( lit('*') | ( -(no_case["DISTINCT"]) >> expression ) )
          >> ')'
          ;

  concat =   no_case["CONCAT"] >> '('
          >> expression >> ',' >> ( expression % ',' )
          >> ')'
          ;
  
  distinct =    no_case["DISTINCT"]
             >> expression
             ; 

  sum =   no_case["SUM"]
       >> '(' >> columnName >> ')'
       ;

  inet_ntoa =    no_case["INET_NTOA"]
              >> '(' >> expression >> ')'
              ;

  inet_aton =    no_case["INET_ATON"]
              >> '(' >> expression >> ')'
              ;

  dateUnit =   no_case["MINUTE"]
             | no_case["SECOND"]
             | no_case["HOUR"]
             | no_case["DAY"]
             ;

  dateAdd =  no_case["DATE_ADD"] >> '('
          >> expression >> ','
          >> lexeme[ no_case["INTERVAL"] >> +space ] >> expression
          >> dateUnit
          >> ')'
          ;

  dateFormat =    no_case["DATE_FORMAT"] >> '('
               >> '(' >> expression >> ')' >> ',' >> string 
               >> ')'
               ;

  Case     =    +(
                       no_case["CASE"] >> expression
                    >> no_case["WHEN"] >> expression
                    >> no_case["THEN"] >> expression
                 )
             >> -(
                       no_case["ELSE"] >> expression
                 )
             >>       no_case["END"]
             ;

  CaseWhen =    +(
                       no_case["CASE"] 
                    >> no_case["WHEN"] >> expression
                    >> no_case["THEN"] >> expression
                 )
             >> -(
                       no_case["ELSE"] >> expression
                 )
             >>       no_case["END"]
             ;

  expression   = andCondition % ( lexeme[ no_case["OR"]  >> +space ] );

  andCondition = condition    % ( lexeme[ no_case["AND"] >> +space ] ) ;

  condition =   ( operand >> - conditionRightHandSide )
              | ( no_case["NOT"] >> condition )
              | ( no_case["EXISTS"] >> '(' >> select >> ')' )
              ;

  operand = summand % lit("||") ;

  summand = factor  % ( lit('+') | lit('-') ) ;

  factor  = term    % ( lit('*') | lit('/') ) ;

  compare = lit("<>") | lit("<=") | lit(">=") | lit("!=")
                      | lit("=" ) | lit(">" ) | lit("<" )
            ;                   

  conditionRightHandSide =
          ( compare >> (   operand
                         | ( ( no_case["ALL"] | no_case["ANY"] | no_case["SOME"] )
                              >> '(' >> select >> ')'
                           )
                       )
          )                       
       |  ( no_case["IS"] >> -(no_case["NOT"]) >> no_case["NULL"] )
       |  ( no_case["IS"] >> -(no_case["NOT"]) 
                          >> -(no_case["DISTINCT"] >> no_case["FROM"])
                          >> operand 
          )
       |  ( no_case["BETWEEN"] >> operand >> no_case["AND"] >> operand )
       |  ( no_case["IN"] >> '(' >> (   (select)
                                      | (expression % ',') ) >> ')' 
          )
       |  ( - no_case["NOT"] >> no_case["LIKE"] >> operand 
                              >> -( no_case["ESCAPE"] >> string ) 
          )
       |  ( - no_case["NOT"] >> no_case["REGEXP"] >> operand )

       ;

     
  order =    (int_ | expression)
          >> -(no_case["ASC"] | no_case["DESC"])
          >> -(no_case["NULLS"] >> (no_case["FIRST"] | no_case["LAST"]))
          ;

    
  selectExpression =   '*'
                     | ( tableAlias >> lit('.') >> lit('*') )
                     | ( expression >> -( -no_case["AS"] >> columnAlias ) )
                     ;

  tableExpression =  (  
                          ( -( schemaName >> '.' ) >> tableName ) 
                       |  ( '(' >> select >> ')' )  
                     )
                     >> -( -no_case["AS"] >> newTableAlias )
                     >> *(
                             (
                                ((no_case["LEFT"] | no_case["RIGHT"]) >> -no_case["OUTER"])
                              | -no_case["INNER"]
                              | no_case["CROSS"]
                              | no_case["NATURAL"]
                             )
                          >> no_case["JOIN"]
                          >> tableExpression
                          >> -(no_case["ON"] >> expression)
                         )
                    ;
                
/*
 *  SELECT statement
 */
  select =    no_case["SELECT"]
           >> -( no_case["TOP"] >> term )
           >> -( no_case["DISTINCT"] | no_case["ALL"] )
           >>  ( selectExpression % ',' )
           >> no_case["FROM"]
           >>  ( tableExpression % ',' )
           >> -( no_case["WHERE"] >> expression )
           >> -( (no_case["GROUP BY"] >> expression) % ',' )
           >> -( no_case["HAVING"] >> expression )
           >> -( 
                 (   
                     ( no_case["UNION"] >> -(no_case["ALL"]) )
                   | ( no_case["MINUS"] )
                   | ( no_case["EXCEPT"] )
                   | ( no_case["INTERSECT"] )
                 ) 
                 >> select
               )
           >> -( (no_case["ORDER BY"] >> order) % ',' )
           >> -( 
                    (no_case["LIMIT"] >> expression)
                >> -(no_case["OFFSET"] >> expression)
                >> -(no_case["SAMPLE_SIZE"] >> rowCountInt)         
               )
           >> -no_case["FOR UPDATE"]
           ;


/*
 *  INSERT statement
 */ 

  insert =    no_case["INSERT INTO"]
           >> tableName
           >> -('(' >> (columnName % ',') >> ')')
           >> (
                 (
                      no_case["VALUES"] 
                   >> (( 
                           '('
                        >> (no_case["DEFAULT"] | (expression % ','))
                        >> ')'
                      ) % ',')
                 )
               | (   select
                  >> -(   no_case["ON DUPLICATE KEY UPDATE"]
                       >> ((   -(tableAlias>>'.')
                            >> columnName 
                            >> '=' 
                            >> expression) % ',') 
                          )         
                      )
                 )
            ;


/*
 *  UPDATE statement
 */
  update =    no_case["UPDATE"]
           >> tableName
           >> -(-no_case["AS"] >> newTableAlias)
           >> no_case["SET"]
           >> (
                (    columnName
                  >> '='
                  >> (no_case["DEFAULT"] | expression)
                ) % ','
              )
           >> -(no_case["WHERE"] >> expression)
           ;

/*
 *  DELETE statement
 */
  Delete =    no_case["DELETE FROM"]
           >> tableName
           >> -(no_case["WHERE"] >> expression)
           ;


/*
 *  TRUNCATE statement
 */
  truncate =    no_case["TRUNCATE"]
             >> -no_case["TABLE"]
             >> tableAlias
             ;


/*
 *  SHOW TABLE STATUS statement
 */
  showTableStatus =   no_case["SHOW TABLE STATUS"]
                   >> (
                          ((no_case["FROM"] | no_case["IN"]) >> newTableAlias)
                        | (
                              (no_case["LIKE"] >> "'" >> tableAlias >> "'")
                            | (no_case["WHERE"] >> expression)
                          )
                      )
                   ;


/*
 *  REPLACE statement
 */
  replace =   no_case["REPLACE"]
           >> -(no_case["LOW_PRIORITY"] | no_case["DELAYED"])
           >> no_case["INTO"]
           >> tableAlias
           >> -('(' >> (columnName % ',') >> ')')
           >> (no_case["VALUES"] | no_case["VALUE"])
           >> lit('(')
           >> ( (expression % ',') | no_case["DEFAULT"] )
           >> lit(')')
           ;



  start =  select
         | insert
         | update
         | Delete
         | truncate
         | showTableStatus
         | replace
         ;

}

bool Parser::Parse(std::string const & str)
{
    using boost::spirit::ascii::space;
    typedef std::string::const_iterator iterator_type;
    typedef sqlparser<iterator_type> sqlparser;

    sqlparser g; // Our grammar

    std::string::const_iterator iter = str.begin();
    std::string::const_iterator end = str.end();

    bool r = phrase_parse(iter, end, g, space);

    if (r && iter == end)
        return true;

    return false;
}

} // end of namespace ensighten
