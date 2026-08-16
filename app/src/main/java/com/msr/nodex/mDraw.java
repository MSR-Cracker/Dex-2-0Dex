package com.msr.nodex;

/*
 * Created by aanti
 * 8/15/2026 12:18 AM
 *
 *   ⋆    ႔ ႔
 *     ᠸ^ ^ ⸝⸝
 *       |、˜〵
 *       じしˍ,)⁐̤ᐷ
 *
 * Fox Mode 🍺
 */

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.view.View;

import androidx.annotation.NonNull;

public class mDraw extends View {

    int mSize;
    private Paint mBLUE;

    // [0][0] = X
    // [0][1] = Y

    /**mAngle
     |
     +---- [0]
            |
            +---- [0] = X
                   |
                   +---- [1] = Y*/
    private float[][] mAngle =
            {
            {
                0f, 0f
            }
            };

    mDraw(Context thiz, int mSize)
    {
        super(thiz);

        this.mSize = mSize;

        mBLUE = new Paint();
        mBLUE.setColor(getResources().getColor(R.color.blue));
        mBLUE.setAntiAlias(true);
        mBLUE.setStyle(Paint.Style.FILL);
        mBLUE.setStrokeJoin(Paint.Join.ROUND);
        mBLUE.setStrokeCap(Paint.Cap.ROUND);
    }

    public void setCircleX(float x)
    {
        mAngle[0][0] = x;
        invalidate();
    }

    public void setCircleY(float y)
    {
        mAngle[0][1] = y;
        invalidate();
    }

    @Override
    protected void onDraw(@NonNull Canvas canvas)
    {
        super.onDraw(canvas);

        canvas.drawCircle(mAngle[0][0],  mAngle[0][1], dpi(mSize), mBLUE);
    }

    public int dpi(int x)
    {
        return (int) (x * getResources().getDisplayMetrics().density);
    }
}