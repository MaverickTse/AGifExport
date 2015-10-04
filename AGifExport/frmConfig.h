#pragma once
#include "AGifExport.h"

namespace AGifExport {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
		
	/// <summary>
	/// Summary for frmConfig
	/// </summary>
	public ref class frmConfig : public System::Windows::Forms::Form
	{
	public:
		frmConfig(CONFIG *data)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			iConfig = data;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~frmConfig()
		{
			if (components)
			{
				delete components;
			}
		}
	private: CONFIG *iConfig;
	private: System::Windows::Forms::NumericUpDown^  numfps;
	protected:
	private: System::Windows::Forms::Label^  lblfps;
	private: System::Windows::Forms::Label^  lblloop;
	private: System::Windows::Forms::Label^  lblDitherMethod;
	private: System::Windows::Forms::Label^  lblColorLimit;
	private: System::Windows::Forms::Label^  lblFuzz;
	private: System::Windows::Forms::NumericUpDown^  numloop;
	private: System::Windows::Forms::ComboBox^  cbDitherMethod;
	private: System::Windows::Forms::NumericUpDown^  numcolor;
	private: System::Windows::Forms::NumericUpDown^  numfuzz;
	private: System::Windows::Forms::Button^  btnOK;
	private: System::Windows::Forms::Button^  btnCancel;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::CheckBox^  cbDropLocal;
	private: System::Windows::Forms::CheckBox^  cbAdaptive;
	private: System::Windows::Forms::Button^  btnDefault;
	private: System::Windows::Forms::Button^  btnRichColor;
	private: System::Windows::Forms::Button^  btnHC;
	private: System::Windows::Forms::CheckBox^  cbMakeTransp;
	private: System::Windows::Forms::ColorDialog^  colorDialogTransparent;
	private: System::Windows::Forms::Button^  btnColorSelect;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->numfps = (gcnew System::Windows::Forms::NumericUpDown());
			this->lblfps = (gcnew System::Windows::Forms::Label());
			this->lblloop = (gcnew System::Windows::Forms::Label());
			this->lblDitherMethod = (gcnew System::Windows::Forms::Label());
			this->lblColorLimit = (gcnew System::Windows::Forms::Label());
			this->lblFuzz = (gcnew System::Windows::Forms::Label());
			this->numloop = (gcnew System::Windows::Forms::NumericUpDown());
			this->cbDitherMethod = (gcnew System::Windows::Forms::ComboBox());
			this->numcolor = (gcnew System::Windows::Forms::NumericUpDown());
			this->numfuzz = (gcnew System::Windows::Forms::NumericUpDown());
			this->btnOK = (gcnew System::Windows::Forms::Button());
			this->btnCancel = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->cbDropLocal = (gcnew System::Windows::Forms::CheckBox());
			this->cbAdaptive = (gcnew System::Windows::Forms::CheckBox());
			this->btnDefault = (gcnew System::Windows::Forms::Button());
			this->btnRichColor = (gcnew System::Windows::Forms::Button());
			this->btnHC = (gcnew System::Windows::Forms::Button());
			this->cbMakeTransp = (gcnew System::Windows::Forms::CheckBox());
			this->colorDialogTransparent = (gcnew System::Windows::Forms::ColorDialog());
			this->btnColorSelect = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numfps))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numloop))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numcolor))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numfuzz))->BeginInit();
			this->SuspendLayout();
			// 
			// numfps
			// 
			this->numfps->Location = System::Drawing::Point(111, 16);
			this->numfps->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 30, 0, 0, 0 });
			this->numfps->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 1, 0, 0, 0 });
			this->numfps->Name = L"numfps";
			this->numfps->Size = System::Drawing::Size(58, 22);
			this->numfps->TabIndex = 0;
			this->numfps->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 15, 0, 0, 0 });
			// 
			// lblfps
			// 
			this->lblfps->AutoSize = true;
			this->lblfps->Font = (gcnew System::Drawing::Font(L"PMingLiU", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(136)));
			this->lblfps->Location = System::Drawing::Point(11, 21);
			this->lblfps->Name = L"lblfps";
			this->lblfps->Size = System::Drawing::Size(58, 12);
			this->lblfps->TabIndex = 1;
			this->lblfps->Text = L"Drop every";
			this->lblfps->Click += gcnew System::EventHandler(this, &frmConfig::lblfps_Click);
			// 
			// lblloop
			// 
			this->lblloop->AutoSize = true;
			this->lblloop->Location = System::Drawing::Point(11, 55);
			this->lblloop->Name = L"lblloop";
			this->lblloop->Size = System::Drawing::Size(36, 12);
			this->lblloop->TabIndex = 2;
			this->lblloop->Text = L"Loop#";
			this->lblloop->Click += gcnew System::EventHandler(this, &frmConfig::lblloop_Click);
			// 
			// lblDitherMethod
			// 
			this->lblDitherMethod->AutoSize = true;
			this->lblDitherMethod->Location = System::Drawing::Point(11, 89);
			this->lblDitherMethod->Name = L"lblDitherMethod";
			this->lblDitherMethod->Size = System::Drawing::Size(73, 12);
			this->lblDitherMethod->TabIndex = 3;
			this->lblDitherMethod->Text = L"Dither Method";
			this->lblDitherMethod->Click += gcnew System::EventHandler(this, &frmConfig::lblDitherMethod_Click);
			// 
			// lblColorLimit
			// 
			this->lblColorLimit->AutoSize = true;
			this->lblColorLimit->Location = System::Drawing::Point(11, 123);
			this->lblColorLimit->Name = L"lblColorLimit";
			this->lblColorLimit->Size = System::Drawing::Size(60, 12);
			this->lblColorLimit->TabIndex = 4;
			this->lblColorLimit->Text = L"Color Limit";
			this->lblColorLimit->Click += gcnew System::EventHandler(this, &frmConfig::lblColorLimit_Click);
			// 
			// lblFuzz
			// 
			this->lblFuzz->AutoSize = true;
			this->lblFuzz->Location = System::Drawing::Point(11, 157);
			this->lblFuzz->Name = L"lblFuzz";
			this->lblFuzz->Size = System::Drawing::Size(66, 12);
			this->lblFuzz->TabIndex = 5;
			this->lblFuzz->Text = L"Fuzziness(%)";
			this->lblFuzz->Click += gcnew System::EventHandler(this, &frmConfig::lblFuzz_Click);
			// 
			// numloop
			// 
			this->numloop->Location = System::Drawing::Point(111, 50);
			this->numloop->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 99, 0, 0, 0 });
			this->numloop->Name = L"numloop";
			this->numloop->Size = System::Drawing::Size(120, 22);
			this->numloop->TabIndex = 6;
			// 
			// cbDitherMethod
			// 
			this->cbDitherMethod->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cbDitherMethod->FormattingEnabled = true;
			this->cbDitherMethod->Items->AddRange(gcnew cli::array< System::Object^  >(22) {
				L"Burke", L"Floyd-Steinburg", L"JJN", L"Ordered",
					L"Sierra3", L"Stucki", L"Floyd(ImageMagick)", L"None", L"IM:Checker", L"IM:DisperseOrdered2x2", L"IM:DisperseOrdered3x3", L"IM:DisperseOrdered8x8",
					L"IM:Halftone4x4 angled", L"IM:Halftone6x6 angled", L"IM:Halftone8x8 angled", L"IM:Halftone4x4 ortho", L"IM:Halftone6x6 ortho",
					L"IM:Halftone8x8 ortho", L"IM:Circle5x5 black", L"IM:Circle5x5 white", L"IM:Circle7x7 black", L"IM:Circle7x7 white"
			});
			this->cbDitherMethod->Location = System::Drawing::Point(111, 85);
			this->cbDitherMethod->Name = L"cbDitherMethod";
			this->cbDitherMethod->Size = System::Drawing::Size(163, 20);
			this->cbDitherMethod->TabIndex = 7;
			this->cbDitherMethod->SelectedIndexChanged += gcnew System::EventHandler(this, &frmConfig::cbDitherMethod_SelectedIndexChanged);
			// 
			// numcolor
			// 
			this->numcolor->Location = System::Drawing::Point(111, 118);
			this->numcolor->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 256, 0, 0, 0 });
			this->numcolor->Minimum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 4, 0, 0, 0 });
			this->numcolor->Name = L"numcolor";
			this->numcolor->Size = System::Drawing::Size(120, 22);
			this->numcolor->TabIndex = 8;
			this->numcolor->Value = System::Decimal(gcnew cli::array< System::Int32 >(4) { 64, 0, 0, 0 });
			// 
			// numfuzz
			// 
			this->numfuzz->Location = System::Drawing::Point(111, 152);
			this->numfuzz->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 50, 0, 0, 0 });
			this->numfuzz->Name = L"numfuzz";
			this->numfuzz->Size = System::Drawing::Size(120, 22);
			this->numfuzz->TabIndex = 9;
			// 
			// btnOK
			// 
			this->btnOK->Location = System::Drawing::Point(37, 278);
			this->btnOK->Name = L"btnOK";
			this->btnOK->Size = System::Drawing::Size(75, 23);
			this->btnOK->TabIndex = 10;
			this->btnOK->Text = L"OK";
			this->btnOK->UseVisualStyleBackColor = true;
			this->btnOK->Click += gcnew System::EventHandler(this, &frmConfig::btnOK_Click);
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(160, 278);
			this->btnCancel->Name = L"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(75, 23);
			this->btnCancel->TabIndex = 11;
			this->btnCancel->Text = L"Cancel";
			this->btnCancel->UseVisualStyleBackColor = true;
			this->btnCancel->Click += gcnew System::EventHandler(this, &frmConfig::btnCancel_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(185, 21);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(36, 12);
			this->label1->TabIndex = 12;
			this->label1->Text = L"frames";
			// 
			// cbDropLocal
			// 
			this->cbDropLocal->AutoSize = true;
			this->cbDropLocal->Location = System::Drawing::Point(11, 185);
			this->cbDropLocal->Name = L"cbDropLocal";
			this->cbDropLocal->Size = System::Drawing::Size(235, 16);
			this->cbDropLocal->TabIndex = 13;
			this->cbDropLocal->Text = L"Drop local colormaps (smaller file, less color)";
			this->cbDropLocal->UseVisualStyleBackColor = true;
			// 
			// cbAdaptive
			// 
			this->cbAdaptive->AutoSize = true;
			this->cbAdaptive->Location = System::Drawing::Point(11, 208);
			this->cbAdaptive->Name = L"cbAdaptive";
			this->cbAdaptive->Size = System::Drawing::Size(303, 16);
			this->cbAdaptive->TabIndex = 14;
			this->cbAdaptive->Text = L"Adaptive frame insert (better compression for simple image)";
			this->cbAdaptive->UseVisualStyleBackColor = true;
			// 
			// btnDefault
			// 
			this->btnDefault->Location = System::Drawing::Point(319, 31);
			this->btnDefault->Name = L"btnDefault";
			this->btnDefault->Size = System::Drawing::Size(75, 23);
			this->btnDefault->TabIndex = 15;
			this->btnDefault->Text = L"Default";
			this->btnDefault->UseVisualStyleBackColor = true;
			this->btnDefault->Click += gcnew System::EventHandler(this, &frmConfig::btnDefault_Click);
			// 
			// btnRichColor
			// 
			this->btnRichColor->Location = System::Drawing::Point(319, 83);
			this->btnRichColor->Name = L"btnRichColor";
			this->btnRichColor->Size = System::Drawing::Size(75, 23);
			this->btnRichColor->TabIndex = 16;
			this->btnRichColor->Text = L"RichColor";
			this->btnRichColor->UseVisualStyleBackColor = true;
			this->btnRichColor->Click += gcnew System::EventHandler(this, &frmConfig::btnRichColor_Click);
			// 
			// btnHC
			// 
			this->btnHC->Location = System::Drawing::Point(319, 135);
			this->btnHC->Name = L"btnHC";
			this->btnHC->Size = System::Drawing::Size(75, 23);
			this->btnHC->TabIndex = 17;
			this->btnHC->Text = L"SmallestFile";
			this->btnHC->UseVisualStyleBackColor = true;
			this->btnHC->Click += gcnew System::EventHandler(this, &frmConfig::btnHC_Click);
			// 
			// cbMakeTransp
			// 
			this->cbMakeTransp->AutoSize = true;
			this->cbMakeTransp->Location = System::Drawing::Point(11, 234);
			this->cbMakeTransp->Name = L"cbMakeTransp";
			this->cbMakeTransp->Size = System::Drawing::Size(153, 16);
			this->cbMakeTransp->TabIndex = 18;
			this->cbMakeTransp->Text = L"Make this color transparent:";
			this->cbMakeTransp->UseVisualStyleBackColor = true;
			// 
			// btnColorSelect
			// 
			this->btnColorSelect->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->btnColorSelect->Location = System::Drawing::Point(171, 230);
			this->btnColorSelect->Name = L"btnColorSelect";
			this->btnColorSelect->Size = System::Drawing::Size(89, 23);
			this->btnColorSelect->TabIndex = 19;
			this->btnColorSelect->Text = L"Select Color";
			this->btnColorSelect->UseVisualStyleBackColor = true;
			this->btnColorSelect->Click += gcnew System::EventHandler(this, &frmConfig::btnColorSelect_Click);
			// 
			// frmConfig
			// 
			this->AcceptButton = this->btnOK;
			this->AutoScaleDimensions = System::Drawing::SizeF(96, 96);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Dpi;
			this->CancelButton = this->btnCancel;
			this->ClientSize = System::Drawing::Size(418, 317);
			this->Controls->Add(this->btnColorSelect);
			this->Controls->Add(this->cbMakeTransp);
			this->Controls->Add(this->btnHC);
			this->Controls->Add(this->btnRichColor);
			this->Controls->Add(this->btnDefault);
			this->Controls->Add(this->cbAdaptive);
			this->Controls->Add(this->cbDropLocal);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->numfuzz);
			this->Controls->Add(this->numcolor);
			this->Controls->Add(this->cbDitherMethod);
			this->Controls->Add(this->numloop);
			this->Controls->Add(this->lblFuzz);
			this->Controls->Add(this->lblColorLimit);
			this->Controls->Add(this->lblDitherMethod);
			this->Controls->Add(this->lblloop);
			this->Controls->Add(this->lblfps);
			this->Controls->Add(this->numfps);
			this->Name = L"frmConfig";
			this->Text = L"Gif Settings";
			this->Load += gcnew System::EventHandler(this, &frmConfig::frmConfig_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numfps))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numloop))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numcolor))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numfuzz))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		

	private: System::Void frmConfig_Load(System::Object^  sender, System::EventArgs^  e) {
		//TODO:
		this->numfps->Value = System::Decimal(this->iConfig->fps);
		this->numloop->Value = System::Decimal(this->iConfig->loop);
		this->numcolor->Value = System::Decimal(this->iConfig->color);
		this->numfuzz->Value = System::Decimal(this->iConfig->fuzz);
		this->cbDitherMethod->SelectedIndex = (int)this->iConfig->method;
		this->cbAdaptive->Checked = this->iConfig->adaptiveinsert;
		this->cbDropLocal->Checked = this->iConfig->droplocal;
		this->cbMakeTransp->Checked = this->iConfig->maketransparent;
		this->colorDialogTransparent->Color.FromArgb(this->iConfig->t_alpha, this->iConfig->t_red, this->iConfig->t_green, this->iConfig->t_blue);
		this->btnColorSelect->ForeColor = this->colorDialogTransparent->Color;
	}
private: System::Void lblloop_Click(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void lblFuzz_Click(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void lblColorLimit_Click(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void lblDitherMethod_Click(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void lblfps_Click(System::Object^  sender, System::EventArgs^  e) {
	}
private: System::Void numfps_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
	
}
private: System::Void btnCancel_Click(System::Object^  sender, System::EventArgs^  e) {
	this->Close();
}
private: System::Void btnOK_Click(System::Object^  sender, System::EventArgs^  e) {
	//save to global setting variable then close dialog
	this->iConfig->fps = Decimal::ToInt32(this->numfps->Value);
	this->iConfig->loop = Decimal::ToInt32(this->numloop->Value);
	this->iConfig->color = Decimal::ToInt32(this->numcolor->Value);
	this->iConfig->fuzz = Decimal::ToInt32(this->numfuzz->Value);
	this->iConfig->method = (Dither_Method)this->cbDitherMethod->SelectedIndex;
	this->iConfig->adaptiveinsert = this->cbAdaptive->Checked;
	this->iConfig->droplocal = this->cbDropLocal->Checked;
	this->iConfig->maketransparent = this->cbMakeTransp->Checked;
	System::Drawing::Color colorbuff = this->colorDialogTransparent->Color;
	this->iConfig->t_alpha = colorbuff.A;
	this->iConfig->t_red = colorbuff.R;
	this->iConfig->t_green = colorbuff.G;
	this->iConfig->t_blue = colorbuff.B;
	this->Close();
}
private: System::Void btnDefault_Click(System::Object^  sender, System::EventArgs^  e) {
	this->numfps->Value = 2;
	this->numloop->Value = 0;
	this->numcolor->Value = 128;
	this->numfuzz->Value = 3;
	this->cbDitherMethod->SelectedIndex = (int)Dither_Method::FloydIM;
	this->cbAdaptive->Checked = false;
	this->cbDropLocal->Checked = true;
}
private: System::Void btnRichColor_Click(System::Object^  sender, System::EventArgs^  e) {
	this->numfps->Value = 3;
	this->numloop->Value = 0;
	this->numcolor->Value = 256;
	this->numfuzz->Value = 3;
	this->cbDitherMethod->SelectedIndex = (int)Dither_Method::Stucki;
	this->cbAdaptive->Checked = true;
	this->cbDropLocal->Checked = false;
}
private: System::Void btnHC_Click(System::Object^  sender, System::EventArgs^  e) {
	this->numfps->Value = 2;
	this->numloop->Value = 0;
	this->numcolor->Value = 256;
	this->numfuzz->Value = 5;
	this->cbDitherMethod->SelectedIndex = (int)Dither_Method::imo33;
	this->cbAdaptive->Checked = true;
	this->cbDropLocal->Checked = true;
}
private: System::Void cbDitherMethod_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	/*
	if (this->cbDitherMethod->SelectedIndex > 8)
	{
		//this->cbDropLocal->Checked = false;
		//this->cbDropLocal->Enabled = false;
		this->numcolor->Value = 256;
		this->numcolor->Enabled = false;
	}
	else
	{
		//this->cbDropLocal->Enabled = true;
		this->numcolor->Enabled = true;
	}*/
}

private: System::Void btnColorSelect_Click(System::Object^  sender, System::EventArgs^  e) {
	this->colorDialogTransparent->ShowDialog();
	btnColorSelect->ForeColor = this->colorDialogTransparent->Color;
}

};

}
