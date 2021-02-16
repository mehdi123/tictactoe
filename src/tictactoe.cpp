#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ID_BP1 100
#define ID_BP2 101
#define ID_BP3 102
#define ID_BP4 103
#define ID_BP5 104
#define ID_BP6 105
#define ID_BP7 106
#define ID_BP8 107
#define ID_BP9 108

#define ID_RDBOX 109

class TicTacToeFrame;
class tttAlgo;
class TicTacToeApp:public wxApp
{
public:
  bool OnInit (void);
};

class TicTacToeFrame:public wxFrame
{
private:
  bool priorityFlag;

public:
  TicTacToeFrame (wxFrame * frame, const wxString & title,
		  const wxPoint & pos, const wxSize & size);
  //  void ShowNextMove(int mine, int machine);
  void OnRadio (wxCommandEvent &);
  void RadioState (bool);
  void SetCellBmp(int , int);
  void Reset();
  void OnClick(wxCommandEvent&);
public:
  wxBitmapButton * cell[9];
  wxBitmap *me, *machine, *free;
  wxRadioBox *radioBox;
  bool clicked;

  tttAlgo *algo;
  DECLARE_EVENT_TABLE ()
};

IMPLEMENT_APP (TicTacToeApp)
class tttStaticBitmap:public wxStaticBitmap
  {
  private:
    int which;
  public:
    tttStaticBitmap (wxFrame * parent, int id, wxBitmap & label,
		     int which);
    void OnClick (wxMouseEvent &);
    DECLARE_EVENT_TABLE ()
};
class tttAlgo
{
private:
  int first;
  bool mine;
  bool machine;
  int myMove[5];
  int machineMove[5];
  int moves[3][3];
  int imy;
  int imachine;
  int winner;
  void SetNextMachine(int move);
  bool IsWinState();
  int CountCol(int col, int limit, int who);
  int CountRow(int row, int limit, int who);
  int CountDiameter(int which, int limit, int who);
public:
  tttAlgo ();
  void SetNextMe(int move);
  int GetNextMove();
  bool IsAllowed(int cell);
  bool IsFinished();
  int GetWinner();
  void Reset();
};


//App:
bool
TicTacToeApp::OnInit (void)
{
#ifdef __WXMSW__

#endif

  // Create the main frame window
  TicTacToeFrame *frame =
    new TicTacToeFrame ((wxFrame *) NULL, (char *) "TicTacToe",
			wxPoint (50, 50), wxSize (450, 300));

  // Give it an icon
#ifdef __WXMSW__
  frame->SetIcon (wxIcon ("mondrian"));
#endif

  frame->Show (TRUE);

  SetTopWindow (frame);
  return TRUE;
}

// My frame constructor
BEGIN_EVENT_TABLE (TicTacToeFrame, wxFrame)
  EVT_RADIOBOX (ID_RDBOX, TicTacToeFrame::OnRadio)
  EVT_BUTTON(ID_BP1, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP2, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP3, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP4, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP5, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP6, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP7, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP8, TicTacToeFrame::OnClick)
  EVT_BUTTON(ID_BP9, TicTacToeFrame::OnClick)
END_EVENT_TABLE ()

  TicTacToeFrame::TicTacToeFrame (wxFrame * frame, const wxString & title, const wxPoint & pos, const wxSize & size):wxFrame (frame, -1, title, pos,
															      size)
{
  clicked = false;
	
  free = new wxBitmap ("free.bmp", wxBITMAP_TYPE_BMP);
  me = new wxBitmap ("me.bmp", wxBITMAP_TYPE_BMP);
  machine = new wxBitmap ("machine.bmp", wxBITMAP_TYPE_BMP);
  wxGridSizer *
    gs =
    new
    wxGridSizer (3, 3, 5);
  wxBoxSizer *
    bs =
    new
    wxBoxSizer (wxVERTICAL);
  wxString *
    rtitle =
    new
    wxString ("Priority");
  const static
    wxString
    choices[] = {
      "Me",
      "Machine"
    };
  radioBox =
    new wxRadioBox (this, ID_RDBOX, *rtitle, wxDefaultPosition,
		    wxDefaultSize, WXSIZEOF (choices), choices);
  for (int i = 0; i < 9; i++)
    {
      cell[i] =
	new wxBitmapButton (this, ID_BP1 + i, *free);
      printf("\n%d", ID_BP1+i);
      gs->Add (cell[i]);//, i % 3, wxEXPAND);
    }
  bs->Add (radioBox, 0, wxALIGN_CENTER);
  bs->Add (gs, 0, wxALL | wxEXPAND, 5);
  SetSizer (bs);
  SetAutoLayout (true);
  SetSize (155, 220);
  algo = new tttAlgo ();
}

void TicTacToeFrame::SetCellBmp(int mm, int cel){
  switch(mm){
  case 0:
    cell[cel]->SetBitmapLabel(*me);
    break;
  case 1:
    cell[cel]->SetBitmapLabel(*machine);
    break;
  }
}
void
TicTacToeFrame::OnRadio (wxCommandEvent & event)
{
  wxString me (event.GetString ());
  if (!me.CompareTo ("Machine"))
    {
      RadioState (false);
      int i = algo->GetNextMove();
      SetCellBmp(1, i);
    }
}

void
TicTacToeFrame::RadioState (bool state)
{
  if (clicked == false)
    {
      clicked = !state;
      radioBox->Enable (state);
    }
}

void TicTacToeFrame::Reset()
{
  for(int i = 0 ; i < 9 ; i++)
    cell[i]->SetBitmapLabel(*free);
  algo->Reset();
}

void TicTacToeFrame::OnClick(wxCommandEvent& event){
  printf("\n%d",event.GetId());
  
}
tttAlgo::tttAlgo ()
{
  for (int i = 0; i < 9; i++)
    moves[i/3][i%3] = -1;
  imy = imachine = 0;
}

void
tttAlgo::SetNextMe (int move){
}

int tttAlgo::GetNextMove(){
  if(!imachine && !imy){
    time_t t;
    time(&t);
    SetNextMachine(((int)t)%9);
    return ((int)t)%9;
  }
  int move;
  bool state = false;
  int i;
  for(i = 0 ; i < 3 ; i++){
    move = CountRow(i, 2, 0);
    if(move != -1)
      return move;
    else{
      move = CountCol(2, i, 0);
      if(move != -1)
	return move;
    }
  }
  move = CountDiameter(0, 2, 0);
  if(move != -1)
    return move;
  move = CountDiameter(1, 2, 0);
  if(move != -1)
    return move;
  for(int j = 2; j > -1 ; j--){
    for(i = 0 ; i < 3 ; i++){
      move = CountRow(i, j, 1);
      if(move != -1)
	return move;
      else{
	move = CountCol(i, j, 1);
	if(move != -1)
	  return move;
      }
    }
    move = CountDiameter(0, j, 1);
    if(move != -1)
      return move;
    move = CountDiameter(1, j, 1);
    if(move != -1)
      return move;
  }
}
int tttAlgo::CountRow(int row, int limit, int who){
  int count = 0;
  int free, i;
  for(i = 0 ; i < 3 ; i++)
    if(moves[row][i] == who)
      count++;
    else
      free = i;
  if(count == limit)
    return i;
  return -1;
}

int tttAlgo::CountCol(int col, int limit, int who){
  int count = 0;
  int free, i;
  for(i = 0 ; i < 3 ; i++)
    if(moves[i][col] == who)
      count++;
    else
      free = i;
  if(count == limit)
    return i;
  return -1;
} 

int tttAlgo::CountDiameter(int which, int limit, int who){
  int count = 0;
  int move;
  switch(which){
  case 0:
    for(int i = 0 ; i < 3 ; i++)
      if(moves[i][i] == who)
	count++;
      else
	move = i;
    break;
  case 1:
    for(int i = 0 ; i < 3 ; i++)
      if(moves[i][2-i] == who)
	count++;
      else
	move = i;
    break;
  }
  if(count == limit)
    return move;
}
bool tttAlgo::IsAllowed(int cell){
  if(IsFinished())
    return false;
  return moves[cell/3][cell%3] == -1;
}

bool tttAlgo::IsFinished(){
  if(IsWinState())
    return true;
  else
    return imy+imachine == 9;
}

void tttAlgo::SetNextMachine(int move)
{
  if(!IsFinished()){
    moves[move/3][move%3] = 1;
    imachine++;
  }
}

int tttAlgo::GetWinner()
{
  return winner;
}

void tttAlgo::Reset()
{
  for(int i = 0 ; i < 9 ; i++)
    moves[i/3][i%3] = -1;
  winner = -1;
  imy = imachine = 0;
}

bool tttAlgo::IsWinState(){
  for(int i = 0 ; i < 3 ; i++){
    if((moves[i][0] == 0 && moves[i][1] == 0 && moves[i][2] == 0) ||
       (moves[0][i] == 0 && moves[1][i] == 0 && moves[2][i] == 0)){
      winner = 0;
      return true;
    }else if((moves[i][0] == 1 && moves[i][1] == 1 && moves[i][2] == 1)||
	     (moves[0][i] == 1 && moves[1][i] == 1 && moves[2][i] == 1)){
      winner = 1;
      return true;
    } 
  }
  if(moves[0][0] == 0 && moves[1][1] == 0 && moves[2][2] == 0){
    winner = 0;
    return true;
  }
  if(moves[0][0] == 1 && moves[1][1] == 1 && moves[2][2] == 1){
    winner = 1;
    return true;
  }
  if(moves[0][2] == 0 && moves[1][1] == 0 && moves[2][0] == 0){
    winner = 0;
    return true;
  }
  if(moves[0][0] == 1 && moves[1][1] == 1 && moves[2][2] == 1){
    winner = 1;
    return true;
  }
  return false;
}
/*
tttStaticBitmap::tttStaticBitmap (wxFrame * parent, int id,
				    wxBitmap & label,
				    int which):
    wxStaticBitmap (parent, id, label)
{
}

void
tttStaticBitmap::OnClick (wxMouseEvent & WXUNUSED (event))
{
  TicTacToeFrame *frame = (TicTacToeFrame *) wxTheApp->GetTopWindow ();
  frame->RadioState(false);
  printf("\n%d", which);
  if(frame->algo->IsAllowed(which)){
    SetBitmapLabel (*frame->me);
    frame->algo->SetNextMe(which);
    if(!frame->algo->IsFinished())
      frame->SetCellBmp(1, frame->algo->GetNextMove());
    else{
      int winner = frame->algo->GetWinner();
      wxString msg;
      switch(winner){
      case 0:
	msg.Format("%s", "You won!!!");
	break;
      case 1:
	msg.Format("%s", "You lost!!!");
	break;
      }
      wxMessageBox(msg.GetData(), "Result", wxOK|wxICON_INFORMATION, frame);
      frame->RadioState(true);
      frame->Reset();
    }
  }
}
*/
