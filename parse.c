#include "main.h"

extern int _line;
extern struct cur_tok _cur;

value_t *newvalue(value_dt type, void *value)
{
  value_t *val=malloc(sizeof(value_t));
  val->type = type;
  switch(type){
     case DT_STRING:
	     val->s = strdup(value);
	     break;
     case DT_NUMBER:
	     val->n = *((double *)value);
	     break;
     case DT_OBJECT:
	     val->o = value;
	     break;
     case DT_ARRAY:
	     val->a = value;
	     break;
     case DT_TRUE:
	     val->any = value;
	     break;
     case DT_FALSE:
	     val->any = value;
	     break;
     case DT_NULL: 
	     val->any = value;
	     break;
  
  }
  return val;
}

pair_t *newpair(char *name, value_t *v)
{
   pair_t *pair=malloc(sizeof(pair_t));
   pair->next = NULL;
   pair->name = name;
   pair->v = *v;

   return pair;

}

value_t *parse_array(void)
{
    value_t *val;
    array_t *arr= malloc(sizeof(array_t));
    arr->a=NULL;
    arr->length = 0;
    int num = 0;
    if(accept(T_CBRAC)){
	
       return newvalue(DT_ARRAY,arr);

    }
    val=parse_value();
    //if(val->type=DT_NUMBER)
    //	    printf("Number: %lf\n",val->n);
    arr->a = val;
    arr->length= 1;
    num++;
    while(accept(T_COMMA) && !accept(T_CBRAC)){
	val = parse_value();
        //if(val->type=DT_NUMBER)
	//	printf("Number: %lf\n",val->n);
        num++;
        arr->a=realloc(arr->a,num*sizeof(value_t));
	arr->a[num-1] = *val;
	//printf("In loop for array\n");
    }
    accept(T_CBRAC);
    //printf("Out of loop\n");
   arr->length = num;
   val = newvalue(DT_ARRAY,arr);
  //printf("About to return\n"); 
   return val;
}


pair_t *parse_pair(void)
{   pair_t *p=malloc(sizeof(pair_t));
	p->next=NULL;
   // value_t *val;
    char *c;
    //char buf[1024];
    //double n;
    //if(accept(T_COLON)){
    //token_t tok = lex(buf, &n);
    if (_cur.tok != T_STRING){
        return NULL;
    }
    //printf("_cur.buf: %s\n",_cur.buf);
    p->name = strdup(_cur.buf);
    match(T_STRING);
    match(T_COLON);
    p->v = *parse_value();
    //printf("Parsed a value\n");
    //if(p->v.type==DT_STRING)
    //	    printf("key: %s\n",p->v.s);
    //if(p->v.type==DT_NUMBER)
    //	    printf("KEY: %lf\n",p->v.n);
    //p=newpair(_cur.buf, _cur.val);    
    if(accept(T_COMMA)){
        p->next = parse_pair();
        	
    } 
    if(p && !p->next)
	p->next=NULL;	
    return p;
}

value_t *parse_object(void)
{
    value_t *val;
    pair_t *p = parse_pair();
    pair_t *hp = NULL, *ep = NULL;
    while(!accept(T_CCBRACE)){
	p = parse_pair();
    }
    val = newvalue(DT_OBJECT,p);
    //match(T_CCBRACE);
    return val;
}

/**
 * The only function you actually need to have.  Parses a json "value"
 */
value_t *parse_value(void)
{
    value_t *val;
    pair_t *pair;
    //char buf[1024];
    //double n;
    //token_t tok = lex(buf, &n); 
    printf("%d\n",_cur.tok);
    switch(_cur.tok) {
      case T_OBRAC:
	match(T_OBRAC);
	val = parse_array();
	//match(T_CBRAC);
	break;
      case T_OCBRACE:
	match(T_OCBRACE);
	val = parse_object();
        //printf("Try getting }\n");
	break;

      case T_STRING:
        val = newvalue(DT_STRING,_cur.buf);
	match(T_STRING);
	break;
	
      case T_NUMBER:
	val = newvalue(DT_NUMBER,&_cur.val);
	match(T_NUMBER);
	break;
      case T_COMMA:
	match(T_COMMA);
	val = parse_value();
	break;
      case T_TRUE:
	match(T_TRUE);
	bool t = TRUE;
	val = newvalue(DT_TRUE, &t);
	break;
      case T_FALSE:
	match(T_FALSE);
	bool f = FALSE;
	val = newvalue(DT_FALSE, &f);
	break;
      case T_NULL:
	match(T_NULL);
	val = newvalue(DT_NULL, NULL);
	break;

    }



   return val;

}
