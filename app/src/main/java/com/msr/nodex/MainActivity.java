package com.msr.nodex;

import androidx.appcompat.app.AppCompatActivity;
import android.animation.ObjectAnimator;
import android.animation.ValueAnimator;
import android.graphics.Color;
import android.graphics.Path;
import android.os.Bundle;
import android.text.SpannableString;
import android.text.Spanned;
import android.text.style.ForegroundColorSpan;
import android.view.ViewGroup;
import android.view.animation.LinearInterpolator;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("nodex");
    }

    public native String mStringCxx();
    public native int mIntCxx();
    private LinearLayout mLayout;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mLayout = findViewById(R.id.mLinear);
       /* TextView mTv = new TextView(this);
        mTv.setTextSize(50);
        mTv.setTextColor(Color.BLACK);
        mTv.setText(mStringCxx());*/
        FrameLayout mDoor = findViewById(R.id.mDoor);

        mDraw dr = new mDraw(this, 40);

        mDoor.addView(dr, new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT));

        int x =0;
        for (; x < 2;) {

            TextView mTv = new TextView(this);
            mTv.setTextSize(20);

            if (x == 0) {
                String mText = mStringCxx();
                String[] mWords = mText.split(" ");
                mTv.setText(mText);


                mTv.setPadding(0, 5, 0, 5);
                mTv.setTextColor(Color.DKGRAY);

                TextView mSpanTv = mTv;
                ValueAnimator mAnimator = ValueAnimator.ofInt(0, mWords.length);
                mAnimator.setDuration((long) mWords.length * 590);
                ///  cast is important otherwise java don't know what is it
                mAnimator.addUpdateListener(animation -> {
                    int wordCount = (int) animation.getAnimatedValue();
                    SpannableString mSpan = new SpannableString(mText);
                    int mPos = 0;
                    int i = 0;
                    for ( ;i < wordCount;) {
                        int end = mPos + mWords[i].length();
                        if (end <= mText.length()) {
                            mSpan.setSpan(new ForegroundColorSpan(Color.BLUE), mPos, end, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                        }
                        mPos = end + 1;
                    i = i +1 ;
                    }
                    mSpanTv.setText(mSpan);
                });
                mAnimator.start();
            } else {
                mTv.setText(String.valueOf(mIntCxx()));
                mTv.setPadding(0,5,0,5);
                mTv.setTextColor(Color.BLUE);
            }

            mLayout.addView(mTv);
            x= x+1 ;
        }


        mDoor.post(new Runnable() {
            @Override
            public void run() {

                Path path = new Path();

                path.addCircle(mDoor.getWidth() / 2f, mDoor.getHeight() / 2f, 250f, Path.Direction.CW);

                ObjectAnimator mAnim = ObjectAnimator.ofFloat(dr, "circleX", "circleY",path);

                mAnim.setDuration(4000);
                mAnim.setRepeatCount(ValueAnimator.INFINITE);
                mAnim.setInterpolator(new LinearInterpolator());
                mAnim.start();
            }
        });
    }



}