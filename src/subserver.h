//main subserver, prompts for the mode
void subserver(int);

//subserver for single player
void subserver_single(char *, int);

//subserver for collab mode
void subserver_collab(char *, int);

//subserver for competitive mode
void subserver_competitive(char *, int);

//processing games for single player
void process(char *, int);

//processing games for collab mode
void process_collab(char *, int);

//processing games for competitive mode
void process_competitive(char *, int);
