//#include "phdstyle.h"
#include "TStyle.h"
#include "TROOT.h"
/*
// just after root

.L phdstyle.C
gROOT->Reset();
setPhDStyle();

 */
// phdGrid: Turns the grid lines on (true) or off (false)

TStyle *phdStyle;

void phdGrid(bool gridOn) {
  phdStyle->SetPadGridX(gridOn);
  phdStyle->SetPadGridY(gridOn);
}


void setPhDStyle() {

  phdStyle = new TStyle("phdStyle","Style for P-phd");
  
  gROOT->SetStyle("Plain");
  
//For the matrix plots
phdStyle->SetPaintTextFormat("5.3f"); 
phdStyle->SetPalette(1,0);   

// For the canvas:
  phdStyle->SetCanvasBorderMode(0);
  phdStyle->SetCanvasColor(kWhite);
  phdStyle->SetCanvasDefH(600); //Height of canvas
  phdStyle->SetCanvasDefW(600); //Width of canvas
  phdStyle->SetCanvasDefX(0);   //POsition on screen
  phdStyle->SetCanvasDefY(0);

// For the Pad:
  phdStyle->SetPadBorderMode(0);
  // phdStyle->SetPadBorderSize(Width_t size = 1);
  phdStyle->SetPadColor(kWhite);
  phdStyle->SetPadGridX(false);
  phdStyle->SetPadGridY(false);
  phdStyle->SetGridColor(0);
  phdStyle->SetGridStyle(3);
  phdStyle->SetGridWidth(1);

// For the frame:
  phdStyle->SetFrameBorderMode(0);
  phdStyle->SetFrameBorderSize(1);
  phdStyle->SetFrameFillColor(0);
  phdStyle->SetFrameFillStyle(0);
  phdStyle->SetFrameLineColor(1);
  phdStyle->SetFrameLineStyle(1);
  phdStyle->SetFrameLineWidth(1);

//For the fit/function:
  phdStyle->SetOptFit(1);
  phdStyle->SetFitFormat("5.4g");
  phdStyle->SetFuncColor(2);
  phdStyle->SetFuncStyle(1);
  phdStyle->SetFuncWidth(1);

//For the date:
  phdStyle->SetOptDate(0);
 
//For the statistics box:
  phdStyle->SetOptFile(0);
  phdStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  phdStyle->SetStatColor(kWhite);
  phdStyle->SetStatFont(42);
  phdStyle->SetStatFontSize(0.025);
  phdStyle->SetStatTextColor(1);
  phdStyle->SetStatFormat("6.4g");
  phdStyle->SetStatBorderSize(1);
  phdStyle->SetStatH(0.1);
  phdStyle->SetStatW(0.15);

// Margins:
  phdStyle->SetPadTopMargin(0.05);
  phdStyle->SetPadBottomMargin(0.13);
  phdStyle->SetPadLeftMargin(0.16);
  phdStyle->SetPadRightMargin(0.02);

// For the Global title:
  phdStyle->SetOptTitle(0);
  phdStyle->SetTitleFont(42);
  phdStyle->SetTitleColor(1);
  phdStyle->SetTitleTextColor(1);
  phdStyle->SetTitleFillColor(10);
  phdStyle->SetTitleFontSize(0.05);

// For the axis titles:
  phdStyle->SetTitleColor(1, "XYZ");
  phdStyle->SetTitleFont(42, "XYZ");
  phdStyle->SetTitleSize(0.06, "XYZ");
  phdStyle->SetTitleXOffset(0.9);
  phdStyle->SetTitleYOffset(1.25);

// For the axis labels:
  phdStyle->SetLabelColor(1, "XYZ");
  phdStyle->SetLabelFont(42, "XYZ");
  phdStyle->SetLabelOffset(0.007, "XYZ");
  phdStyle->SetLabelSize(0.05, "XYZ");

// For the axis:
  phdStyle->SetAxisColor(1, "XYZ");
  phdStyle->SetStripDecimals(kTRUE);
  phdStyle->SetTickLength(0.03, "XYZ");
  phdStyle->SetNdivisions(510, "XYZ");
  phdStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  phdStyle->SetPadTickY(1);

// Change for log plots:
  phdStyle->SetOptLogx(0);
  phdStyle->SetOptLogy(0);
  phdStyle->SetOptLogz(0);

// Postscript options:
  phdStyle->SetPaperSize(20.,20.);
  
  // phdStyle->SetLineScalePS(Float_t scale = 3);
  // phdStyle->SetLineStyleString(Int_t i, const char* text);
  // phdStyle->SetHeaderPS(const char* header);
  // phdStyle->SetTitlePS(const char* pstitle);

  // phdStyle->SetBarOffset(Float_t baroff = 0.5);
  // phdStyle->SetBarWidth(Float_t barwidth = 0.5);
  // phdStyle->SetPaintTextFormat(const char* format = "g");
  // phdStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // phdStyle->SetTimeOffset(Double_t toffset);
  // phdStyle->SetHistMinimumZero(kTRUE);

  phdStyle->cd();

}
